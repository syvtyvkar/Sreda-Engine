// (c) Nikita Rogalev. All rights reserved.

#pragma once // Защита от множественного включения

#include "Engine/Input/KeyCodes.h"              // Определения клавиш (InputKey)
#include "Engine/Core/Log.h"                    // Логирование
#include "Engine/Core/Utilities/Event.h"                  // Поддержка делегатов
#include "Engine/Input/IInputListen.h"           // Класс-слушатель ввода
#include "Engine/Platform/IWindow.h"            // Интерфейс окна
#include <glm/glm.hpp>                          // Математическая библиотека для векторов


struct GLFWwindow;          //TODO: Убрать!!!!
//using namespace Input;

namespace Engine
{
    ADD_DELEGATE_THREE_PARAMS(DOnKeyPressed, InputKey, int, int)         // Событие нажатия клавиши (key, scancode, mods)
    ADD_DELEGATE_TWO_PARAMS(DOnKeyReleased,InputKey,int)                 // Событие отпускания клавиши (key, mods)
    ADD_DELEGATE_TWO_PARAMS(DOnMouseMoved,float,float)              // Событие перемещения мыши (x, y)
    ADD_DELEGATE_TWO_PARAMS(DOnMouseScrolled,float,float)           // Событие прокрутки колесика (xOffset, yOffset)
    ADD_DELEGATE_TWO_PARAMS(DOnMouseButtonPressed,InputKey,int)          // Событие нажатия кнопки мыши (button, mods)
    ADD_DELEGATE_TWO_PARAMS(DOnMouseButtonReleased,InputKey,int)         // Событие отпускания кнопки мыши (button, mods)
    
    class Window;

    /**
     * @class InputSystem
     * @brief Система ввода (клавиатура, мышь). Реализована как синглтон.
     *
     * Предоставляет статические методы для получения состояния клавиш и мыши,
     * а также делегаты для подписки на события ввода.
     * Поддерживает два режима работы: непосредственный опрос (IsKeyPressed и т.д.)
     * и событийный (через делегаты).
     */
    class InputSystem
    {
    public:
        static void Init(Engine::Window* windowHandle);                     // Инициализация системы ввода.
        static void Shutdown();                                             // Завершение работы системы ввода, освобождение ресурсов.
        static void Update();                                               // Обновление состояния ввода (должно вызываться каждый кадр).

        // Keyboard
        static bool IsKeyPressed(InputKey Key);                             // Проверяет, зажата ли клавиша в текущем кадре.
        static bool IsKeyJustPressed(InputKey Key);                         // Проверяет, была ли клавиша нажата в этом кадре (только что).
        static bool IsKeyJustReleased(InputKey Key);                        // Проверяет, была ли клавиша отпущена в этом кадре.
        static InputState GetKeyState(InputKey Key);                        // Возвращает текущее состояние клавиши (InputState).

        // Mouse
        static bool IsMouseButtonPressed(InputKey Button);                  // Проверяет, зажата ли кнопка мыши.
        static bool IsMouseButtonJustPressed(InputKey Button);              // Проверяет, была ли кнопка мыши нажата в этом кадре.
        static glm::vec2 GetMousePosition();                                // Возвращает текущую позицию курсора мыши в координатах окна.
        static glm::vec2 GetMouseDelta();                                   // Возвращает изменение позиции мыши за последний кадр (дельта).
        static double GetMouseScrollDelta();                                // Возвращает величину прокрутки колесика мыши.
        static float GetMouseSensivity();
        static bool IsInverMoveY();

        // Cursor
        static bool GetCursorVisible();                         
        static int GetCursorMode();                         
        static void SetCursorVisible(bool Visible);                         // Устанавливает видимость курсора.
        static void SetCursorMode(int Mode);                                // Устанавливает режим курсора: 0=Normal, 1=Hidden, 2=Disabled

        // Singleton
        static InputSystem& GetInstance();                                  // Возвращает ссылку на единственный экземпляр InputSystem.

        static void* GetWindowHandle();                                     // Возвращает указатель на нативный дескриптор окна (например, GLFWwindow*).

        IInputListen* GetInputListen() {return m_InputListen.get();};        // Возвращает указатель на объект InputListen (используется для расширенной обработки).

        // Доступ к делегатам для подписки
        DOnKeyPressed& OnKeyPressed() {return s_OnKeyPressed;}
        DOnKeyReleased& OnKeyReleased() {return s_OnKeyReleased;}
        DOnMouseMoved& OnMouseMoved() {return s_OnMouseMoved;}
        DOnMouseScrolled& OnMouseScrolled() {return s_OnMouseScrolled;}
        DOnMouseButtonPressed& OnMouseButtonPressed() {return s_OnMouseButtonPressed;}
        DOnMouseButtonReleased& OnMouseButtonReleased() {return s_OnMouseButtonReleased;}

    protected:
        TUniquePtr<IInputListen> m_InputListen=nullptr;                 // Слушатель ввода
    private:
        InputSystem() = default;
        ~InputSystem() =default;

        void SetKeyState(InputKey Key, InputState State);                   // Устанавливает состояние указанной клавиши.
        InputState GetKeyStateInternal(InputKey Key) const;                 // Внутренний метод получения состояния клавиши.

        static InputSystem* s_InputInstance;                                // Указатель на единственный экземпляр (синглтон).

        // Состояния клавиш: текущее и предыдущее (для определения just pressed/released).
        uint8_t m_keyStates[static_cast<uint32_t>(InputKey::KeyCount)];
        uint8_t m_prevKeyStates[static_cast<uint32_t>(InputKey::KeyCount)];

        // Состояние мыши
        glm::vec2 m_mousePosition = {0,0};                                  // Текущая позиция курсора.
        glm::vec2 m_mouseDelta = {0,0};                                     // Изменение позиции за кадр.
        double m_scrollDelta = 0.0;                                         // Накопленная прокрутка колесика.
        float m_mouseSensivity = 0.2f;                                      // Сенса мыши
        bool m_Invers_Y = true;

        // Дружественный класс Window имеет доступ к приватным методам (например, для обработки).
        friend class Window;

        // Экземпляры делегатов (событий).
        DOnKeyPressed s_OnKeyPressed;
        DOnKeyReleased s_OnKeyReleased;                     // keyCode, mods
        DOnMouseMoved s_OnMouseMoved;                       // x, y
        DOnMouseScrolled s_OnMouseScrolled;                 // xOffset, yOffset
        DOnMouseButtonPressed s_OnMouseButtonPressed;       // button, mods
        DOnMouseButtonReleased s_OnMouseButtonReleased;     // button, mods

    protected:
        void CallOnKeyPressed(InputKey key, int scancode, int mods, InputState State);
        void CallOnMouseMoved(float x, float y);
        void CallOnMouseScrolled(float x, float mods);
        void CallOnMouseButtonPressed(InputKey button, int mods, InputState State);
    };
}