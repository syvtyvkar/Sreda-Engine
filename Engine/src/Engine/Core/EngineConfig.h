// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Защита от множественного включения заголовочного файла
#include <iostream>                     //

//nlohmann-json
#include <nlohmann/json.hpp>            // Библиотека для работы с JSON
#include <string>                       // 
#include <unordered_map>                // 
#include <functional>                   // 
#include <mutex>                        // 

#ifdef ENGINE_CONFIG_EMBEDDED
#include "config_embed.h"               // Сгенерированный header
#endif

#include <algorithm>                    //
#include <unordered_set>                //
///////////////

namespace EngineConfig 
{
    using json = nlohmann::json;
    using namespace Engine;
    using ChangeCallback = std::function<void(const std::string& key, const json& newValue)>;


    const std::string ENINGE_CONFIG_FILE = "/Config/engine.json";

    /**
     * Система конфигов
     * 
     * Обертка над библиотекой nlohmann для работы с конфигурацией. Настройки движка, игры и так далее.
     * 
     * Написано так, чтобы работать с этим было максимально удобно. Система умеет хранить в сгенерированном 
     * файле базовые настройки "из коробки", которые затем пользователь переопределяет в конфигурационных файлах.
     * 
     * Это низкоуровневый класс. Предполагается, что с ним могут работать только высокоуровневые обертки и базовое 
     * приложение (для первичной загрузки)
     */
    class ConfigSystem 
    {
    public:
        static ConfigSystem& Get();                                                     // Синглтон

        // Шаблонные функции
        template<typename T>
        T Get(const std::string& key, const T& defaultValue) const;
        
        template<typename T>
        void Set(const std::string& key, const T& value);

        bool Init(std::string InTypeConfig);                                            // Инициализация, и первичная загрузка файлов.
        bool DeInit();                                                                  // Вызывать при завершении програмы или перед повторной инициализацией. Чистит данные

        void MarkOverridden(const std::string& key);                                    // Пометить ключ как измененный
        bool SaveConfig(const std::string& InTypeConfig);
    private: 
        ConfigSystem()= default;                                                        // Конструктор
        ~ConfigSystem() = default;                                                      // Деструктор стандартный
        ConfigSystem(const ConfigSystem&) = delete;                                     // Не может существовать одновременно две системы, поэтому запрещаем копирование
        ConfigSystem& operator=(const ConfigSystem&) = delete;                          // Не может существовать одновременно две системы, поэтому запрещаем копирование

        void MarkOverriddenKeys(const json& InConfig, const std::string& InPrefix);     // Пометить ключи файла как переопределенные
        size_t CountKeys(const json& j, size_t depth = 0);                              // Сколько ключей?
        const json* GetNestedValue(const std::string& path) const;                      // Получить указатель на значение
        static std::vector<std::string> SplitPath(const std::string& path);             // Разбить путь "a/b/c" на части ["a", "b", "c"]

        static ConfigSystem* s_instance;                                                // Синглтон
        bool m_embeddedLoaded = false;                                                  // Были ли загружены сгенерированные файлы
        json m_data;                                                                    // Текущий JSON-конфиг с данными
        std::unordered_set<std::string> m_overriddenKeys;                               // Пользовательские ключи поверх дефолтных
        mutable std::mutex m_mutex;                                                     // Для потокобезопасности
    };

    template<typename T>
    T ConfigSystem::Get(const std::string& key, const T& defaultValue) const 
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        const json* value = GetNestedValue(key);
        if (!value) 
        {
            return defaultValue;
        }
        try 
        {
            return value->get<T>();
        } 
        catch (...) 
        {
            // Если тип не совпадает — возвращаем дефолт
            return defaultValue;
        }
    }

    template<typename T>
    void ConfigSystem::Set(const std::string& key, const T& value) 
    {
        json jsonValue;
    
        // Конвертируем значение в json (с обработкой специальных типов)
        try 
        {
            jsonValue = json(value);
        }   
        catch (...) 
        {
            // Если конвертация не удалась, пробуем вручную для частых случаев
            if constexpr (std::is_same_v<T, glm::vec2>) 
            {
                jsonValue = {value.x, value.y};
            } 
            else if constexpr (std::is_same_v<T, glm::vec3>) 
            {
                jsonValue = {value.x, value.y, value.z};
            } 
            else if constexpr (std::is_same_v<T, glm::vec4>) 
            {
                jsonValue = {value.x, value.y, value.z, value.w};
            } 
            else 
            {
                ENGINE_LOG_ERROR("Failed to convert value to JSON for key: {}", key);
                return;
            }
        }

        std::string normalizedKey;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
        
            normalizedKey = key;  // Для подписчиков используем исходный ключ
            //SetNestedValue(key, jsonValue);

            auto parts = SplitPath(key);
            if (parts.empty()) return;
    
            json* current = &m_data;
    
            for (size_t i = 0; i < parts.size() - 1; ++i) 
            {
                const std::string& key = parts[i];
        
                // Если ключ не существует или не объект — создаём объект
                if (!current->is_object() || !current->contains(key)) 
                {
                    (*current)[key] = json::object();
                }
        
                current = &current->at(key);
            }
    
            // Устанавливаем финальное значение
            const std::string& lastKey = parts.back();
            (*current)[lastKey] = value;

        }
        ENGINE_LOG_TRACE("Change config key: {}, value: {}", key, value);
        MarkOverridden(key);
    }
}