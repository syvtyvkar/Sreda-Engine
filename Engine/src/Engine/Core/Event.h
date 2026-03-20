// (c) Nikita Rogalev. All rights reserved.

#pragma once // Защита от множественного включения заголовочного файла

#include <string>
#include "Engine/Core/Log.h"                    // Подключение системы логирования
#include <cstdint>                              // Для целочисленных типов фиксированной ширины
#include <functional>                           // Для std::function
#include <memory>                               // Для умных указателей
#include <vector>                               // Для std::vector
#include <unordered_map>                        // Для std::unordered_map
#include <mutex>                                // Для std::mutex, std::lock_guard
#include <atomic>                               // Для std::atomic

namespace Engine
{
    using DelegateHandle = uint64_t;                        // Тип для уникального идентификатора подписки
    constexpr DelegateHandle InvalidDelegateHandle = 0;     // Константа, обозначающая недействительный идентификатор

    /**
     * Multicast делегат (шаблон проектирования "Наблюдатель").
     * Позволяет подписываться на события и уведомлять всех подписчиков.
     * 
     * Args - типы параметров, передаваемых при вызове.
     * 
     * Пример:
     * 
     * // Объявление
     * Delegate<int, float> OnDamage;
     * 
     * // Подписка
     * OnDamage.Subscribe([](int damage, float multiplier) {YOU CODE;});
     * 
     * // Вызов
     * OnDamage.Broadcast(50, 2.0f);
     * 
     * Реализация потокобезопасна (используется мьютекс).
     * Поддерживается приоритетная подписка.
     */

    template<typename... Args>
    class Delegate 
    {
    public:
        using CallbackType = std::function<void(Args...)>;          // Тип функции-обработчика

        Delegate() = default;
        ~Delegate() = default;

        DelegateHandle Subscribe(CallbackType callback) // Добавляет обработчик без приоритета, возвращает уникальный идентификатор для отписки
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            DelegateHandle handle = s_NextHandle.fetch_add(1);                          // Генерация нового уникального handle
            m_Callbacks[handle] = std::move(callback);
            return handle;
        }

        DelegateHandle SubscribeWithPriority(CallbackType callback, int priority = 0) // Добавляет обработчик с указанным приоритетом (чем выше число, тем раньше будет вызван)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            DelegateHandle handle = s_NextHandle.fetch_add(1);
            m_Callbacks[handle] = std::move(callback);
            m_Priorities[handle] = priority;
            SortByPriority();                                                           // Пересортировка списка вызовов по приоритету
            return handle;
        }

        void Unsubscribe(DelegateHandle handle)                                         // Удаляет обработчик по его идентификатору
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Callbacks.erase(handle);
            m_Priorities.erase(handle);
        }

        void Broadcast(Args... args)  // Оповещает всех подписчиков, передавая им аргументы args... Исключения в обработчиках игнорируются (перехватываются и подавляются)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            auto callbacksCopy = m_Callbacks;
            for (const auto& [handle, callback] : callbacksCopy) {
                if (callback) {
                    try {
                        callback(args...);
                    } catch (...) {
                        // Игнорируем ошибки в коллбеках
                    }
                }
            }
        }

        void Clear()  // Удаляет всех подписчиков
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Callbacks.clear();
            m_Priorities.clear();
        }

        size_t GetSubscriberCount() const // Возвращает количество текущих подписчиков
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            return m_Callbacks.size();
        }

        bool HasSubscribers() const {return GetSubscriberCount() > 0;} // Проверяет, есть ли хотя бы один подписчик

        // Операторы для удобства использования
        void operator()(Args... args) { Broadcast(args...); }                               // Вызов как функции
        DelegateHandle operator+=(CallbackType callback) { return Subscribe(callback); }    // Добавление подписчика через +=
        void operator-=(DelegateHandle handle) { Unsubscribe(handle); }                     // Удаление через -=

    private:
        void SortByPriority() // Вспомогательный метод для сортировки m_Callbacks в соответствии с приоритетами
        {
            std::vector<std::pair<DelegateHandle, int>> sortedHandles;
            for (const auto& [handle, priority] : m_Priorities) {
                sortedHandles.emplace_back(handle, priority);
            }
            std::sort(sortedHandles.begin(), sortedHandles.end(),[](const auto& a, const auto& b) { return a.second > b.second; }); // Сортировка по убыванию приоритета
            
            decltype(m_Callbacks) sortedCallbacks;                  // Создаём новую упорядоченную карту обратных вызовов
            for (const auto& [handle, priority] : sortedHandles) 
            {
                if (m_Callbacks.find(handle) != m_Callbacks.end()) 
                {
                    sortedCallbacks[handle] = m_Callbacks[handle];
                }
            }
            m_Callbacks = std::move(sortedCallbacks);
        }

        std::unordered_map<DelegateHandle, CallbackType> m_Callbacks;   // Хранилище обработчиков по handle
        std::unordered_map<DelegateHandle, int> m_Priorities;           // Приоритеты для каждого handle
        mutable std::mutex m_Mutex;                                     // Мьютекс для потокобезопасности (https://ru.ruwiki.ru/wiki/Мьютекс)
        static std::atomic<DelegateHandle> s_NextHandle;                // Генератор уникальных идентификаторов
    };

    template<typename... Args>  // Инициализация статического счётчика идентификаторов (начинаем с 1, так как 0 зарезервирован как Invalid)
    std::atomic<DelegateHandle> Delegate<Args...>::s_NextHandle{1};

/*********************************************************/
// Набор макросов для удобного объявления конкретных типов делегатов (синглтонов) с разным числом параметров.

    // Делегат без параметров (синглтон)
    #define ADD_DELEGATE(DelegateName) \                        
        class DelegateName : public Engine::Delegate<> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<>; \
        };

    // Делегат с одним параметром (синглтон)
    #define ADD_DELEGATE_ONE_PARAM(DelegateName, ParamType1) \
        class DelegateName : public Engine::Delegate<ParamType1> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<ParamType1>; \
            using Param1 = ParamType1; \
        };

    // Делегат с двумя параметрами (синглтон)
    #define ADD_DELEGATE_TWO_PARAMS(DelegateName, ParamType1, ParamType2) \
        class DelegateName : public Engine::Delegate<ParamType1, ParamType2> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<ParamType1, ParamType2>; \
            using Param1 = ParamType1; \
            using Param2 = ParamType2; \
        };

    // Делегат с тремя параметрами (синглтон)
    #define ADD_DELEGATE_THREE_PARAMS(DelegateName, ParamType1, ParamType2, ParamType3) \
        class DelegateName : public Engine::Delegate<ParamType1, ParamType2, ParamType3> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<ParamType1, ParamType2, ParamType3>; \
            using Param1 = ParamType1; \
            using Param2 = ParamType2; \
            using Param3 = ParamType3; \
        };

    // Делегат с четырьмя параметрами (синглтон)
    #define ADD_DELEGATE_FOUR_PARAMS(DelegateName, ParamType1, ParamType2, ParamType3, ParamType4) \
        class DelegateName : public Engine::Delegate<ParamType1, ParamType2, ParamType3, ParamType4> { \
        public: \
            static DelegateName& Get() { \
                static DelegateName instance; \
                return instance; \
            } \
            using Super = Engine::Delegate<ParamType1, ParamType2, ParamType3, ParamType4>; \
            using Param1 = ParamType1; \
            using Param2 = ParamType2; \
            using Param3 = ParamType3; \
            using Param4 = ParamType4; \
        };

/****************************************************************/

    // Макросы для привязки методов класса к делегату (используют std::bind)
    #define BIND_DELEGATE_METHOD(DelegateClass, Method) \
        std::bind(&std::decay_t<decltype(*this)>::Method, this, \
                  std::placeholders::_1, std::placeholders::_2, \
                  std::placeholders::_3, std::placeholders::_4)

    // Макрос для создания лямбда-выражения с захватом (просто передаёт список захвата)
    #define BIND_DELEGATE_LAMBDA(...) \
        [__VA_ARGS__]

    // Макрос для объявления события (не синглтон, просто псевдоним делегата)
    #define ADD_EVENT(ClassType, EventName, ...) \
        class EventName : public Engine::Delegate<__VA_ARGS__> { \
        public: \
            using Super = Engine::Delegate<__VA_ARGS__>; \
        };

    // Макросы для объявления multicast делегатов (синонимы ADD_DELEGATE_*, но с другим именем макроса)
    #define ADD_MULTICAST_DELEGATE(DelegateName) \
        class DelegateName : public Engine::Delegate<> { \
        public: \
            using Super = Engine::Delegate<>; \
        };

    #define ADD_MULTICAST_DELEGATE_ONE_PARAM(DelegateName, ParamType1) \
        class DelegateName : public Engine::Delegate<ParamType1> { \
        public: \
            using Super = Engine::Delegate<ParamType1>; \
            using Param1 = ParamType1; \
        };

    #define ADD_MULTICAST_DELEGATE_TWO_PARAMS(DelegateName, ParamType1, ParamType2) \
        class DelegateName : public Engine::Delegate<ParamType1, ParamType2> { \
        public: \
            using Super = Engine::Delegate<ParamType1, ParamType2>; \
            using Param1 = ParamType1; \
            using Param2 = ParamType2; \
        };

}