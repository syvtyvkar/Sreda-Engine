// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Защита от множественного включения

#include "Engine/Core/Types.h"          // Основные типы движка (Vector2, Vector3 и т.д.)
#include "Engine/Input/KeyCodes.h"      // Коды клавиш (InputKey)
//#include "Engine/Input/Input.h"
#include "Engine/Core/Log.h"            // Логирование
#include "Engine/Core/Event.h"          // Базовые события (вероятно, используется в делегатах)
#include <memory>                       // Для std::unique_ptr
#include <string>                       // Для std::string

// Определение типов делегатов для событий ввода, специфичных для InputListen.
ADD_DELEGATE_THREE_PARAMS(DOnKeyPressedListen, int, int, int)       // key, scancode, mods
ADD_DELEGATE_TWO_PARAMS(DOnKeyReleasedListen,int,int)               // key, mods
ADD_DELEGATE_TWO_PARAMS(DOnMouseMovedListen,float,float)            // x, y
ADD_DELEGATE_TWO_PARAMS(DOnMouseScrolledListen,float,float)         // xOffset, yOffset
ADD_DELEGATE_TWO_PARAMS(DOnMouseButtonPressedListen,int,int)        // button, mods
ADD_DELEGATE_TWO_PARAMS(DOnMouseButtonReleasedListen,int,int)       // button, mods

namespace Engine::Input 
{
    /**
     * @class InputListen
     * @brief Абстрактный базовый класс для обработчиков ввода.
     *
     * Предоставляет интерфейс для создания объектов, которые могут подписываться
     * на события ввода и обрабатывать их. Используется для отделения логики обработки
     * ввода от конкретной платформы или API
     *
     * Класс содержит виртуальные методы Init/DeInit и набор делегатов,
     * через которые можно получать уведомления о событиях ввода.
     */
    class InputListen
    {
    public:

        //virtual InputListen() = default;
        virtual ~InputListen() = default;                                                               // Виртуальный деструктор для корректного удаления наследников

        virtual void Init(class InputSystem* InInput) = 0;                                              // Инициализация слушателя.
        virtual void DeInit() = 0;                                                                      // Деинициализация слушателя, освобождение ресурсов.

        // Методы доступа к делегатам для подписки.
        DOnKeyPressedListen& OnKeyPressed() {return s_OnKeyPressedListen;}
        DOnKeyReleasedListen& OnKeyReleased() {return s_OnKeyReleasedListen;}
        DOnMouseMovedListen& OnMouseMoved() {return s_OnMouseMovedListen;}
        DOnMouseScrolledListen& OnMouseScrolled() {return s_OnMouseScrolledListen;}
        DOnMouseButtonPressedListen& OnMouseButtonPressed() {return s_OnMouseButtonPressedListen;}
        DOnMouseButtonReleasedListen& OnMouseButtonReleased() {return s_OnMouseButtonReleasedListen;}

    protected:

        // Защищённые экземпляры делегатов, доступные наследникам.
        DOnKeyPressedListen s_OnKeyPressedListen;
        DOnKeyReleasedListen s_OnKeyReleasedListen;
        DOnMouseMovedListen s_OnMouseMovedListen;
        DOnMouseScrolledListen s_OnMouseScrolledListen;
        DOnMouseButtonPressedListen s_OnMouseButtonPressedListen;
        DOnMouseButtonReleasedListen s_OnMouseButtonReleasedListen;

    };

    /**
     * @class InputListenAPIFactory
     * @brief Фабрика для создания экземпляров InputListen в зависимости от платформы.
     *
     * Позволяет инкапсулировать логику выбора конкретной реализации InputListen
     * (например, InputGLF3 для GLFW) и скрыть её за единым интерфейсом.
     */
    class InputListenAPIFactory 
    {
    public:
        /**
         * @brief Создаёт уникальный указатель на объект InputListen.
         * @return std::unique_ptr<InputListen> Указатель на созданный экземпляр.
         */
        static std::unique_ptr<InputListen> create();
    };
}