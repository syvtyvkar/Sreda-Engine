// (c) Nikita Rogalev. All rights reserved.

#pragma once                                // Защита от множественного включения

#include "Engine/Input/InputListen.h"       // Базовый абстрактный класс для слушателей ввода
#include "Engine/Input/KeyCodes.h"          // Коды клавиш (InputKey)
#include "Engine/Core/Log.h"                // Логирование
#include "Engine/Core/Event.h"              // Базовые события (для делегатов)
#include "Engine/Platform/IWindow.h"        // Интерфейс окна
#include <glm/glm.hpp>                      // Математическая библиотека (векторы)

// Предварительное объявление структуры GLFWwindow (используется для платформозависимых колбэков)
struct GLFWwindow;
using namespace Input;

namespace Engine::Input  
{
    class Window;

    /**
     * @class InputListenGLF3System
     * @brief Реализация InputListen для платформы GLFW.
     *
     * Этот класс является конкретной реализацией абстрактного интерфейса InputListen.
     * Он подключается к окну GLFW, регистрирует колбэки и хранит состояние клавиш и мыши.
     * Предоставляет методы для опроса состояния ввода, а также транслирует события
     * через унаследованные делегаты (OnKeyPressed и т.д.).
     */

    class InputListenGLF3System : public InputListen
    {
    public:
        InputListenGLF3System() = default;
        virtual ~InputListenGLF3System() override;

        // Переопределение виртуальных методов базового класса
        virtual void Init(InputSystem* InInput) override;
        virtual void DeInit() override;

        // Клавиатура
        bool IsKeyPressed(InputKey Key);
        bool IsKeyJustPressed(InputKey Key);
        bool IsKeyJustReleased(InputKey Key);
        InputState GetKeyState(InputKey Key);

        // Мышь
        bool IsMouseButtonPressed(InputKey Button);
        bool IsMouseButtonJustPressed(InputKey Button);
        glm::vec2 GetMousePosition();
        glm::vec2 GetMouseDelta();
        double GetMouseScrollDelta();

        // Управление курсором
        void SetCursorVisible(bool Visible);                    
        void SetCursorMode(int Mode); // 0=Normal, 1=Hidden, 2=Disable

        // Утилиты
        void* GetWindowHandle();                    // Получить нативный дескриптор окна
        InputKey InputKeyFromInt(int IntKey);       // Преобразовать целочисленный код в InputKey

    protected:

    /**
         * @brief Устанавливает состояние указанной клавиши.
         * @param Key Код клавиши.
         * @param State Новое состояние.
         */
        void SetKeyState(InputKey Key, InputState State);
    private:
    /**
         * @brief Внутренний метод получения состояния клавиши.
         * @param Key Код клавиши.
         * @return Текущее состояние клавиши.
         */
        InputState GetKeyStateInternal(InputKey Key) const;

        // Массивы состояний клавиш: текущее и предыдущее (для определения just pressed/released)
        uint8_t m_keyStates[static_cast<uint32_t>(InputKey::KeyCount)];
        uint8_t m_prevKeyStates[static_cast<uint32_t>(InputKey::KeyCount)];

        // Состояние мыши
        glm::vec2 m_mousePosition = {0,0};                  // Текущая позиция курсора
        glm::vec2 m_mouseDelta = {0,0};                     // Изменение позиции за последний кадр
        double m_scrollDelta = 0.0;                         // Накопленная прокрутка колесика

        void* m_windowHandle = nullptr;                     // Указатель на нативный дескриптор окна (GLFWwindow*)

#ifdef ENGINE_GLFW
        // Преобразования между InputKey и GLFW-кодами
        static int ToGLFWKey(InputKey key);
        static InputKey FromGLFWKey(int glfwKey);
        static int ToGLFWMouseButton(InputKey button);
        // Статические функции-обработчики, которые регистрируются в GLFW как колбэки.
        static void OnGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void OnGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void OnGLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void OnGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
#endif
        // Класс Window объявлен другом, чтобы иметь доступ к приватным членам
        friend class Window;
    };
}