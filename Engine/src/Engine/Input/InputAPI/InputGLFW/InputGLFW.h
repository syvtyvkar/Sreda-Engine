// (c) Nikita Rogalev. All rights reserved.

#ifdef ENGINE_WINDOW_GLFW

#pragma once                                // Защита от множественного включения

#include "Engine/Input/IInputListen.h"       // Базовый абстрактный класс для слушателей ввода
#include "Engine/Input/KeyCodes.h"          // Коды клавиш (InputKey)
#include "Engine/Core/Log.h"                // Логирование
#include "Engine/Core/Utilities/Event.h"              // Базовые события (для делегатов)
#include "Engine/Platform/IWindow.h"        // Интерфейс окна
#include <glm/glm.hpp>                      // Математическая библиотека (векторы)
#include <unordered_map>                    // Для мапы инпутов
#include <GLFW/glfw3.h>

// Предварительное объявление структуры GLFWwindow (используется для платформозависимых колбэков)
struct GLFWwindow;
//using namespace Input;

namespace Engine  
{
    class Window;

    /**
    * Маппинг абстрактных Key → GLFW коды_*
    */
    inline std::unordered_map<int, InputKey> GLFWInputToEngineMap =
    {
        {GLFW_KEY_A, InputKey::A},
        {GLFW_KEY_B, InputKey::B},
        {GLFW_KEY_C, InputKey::C},
        {GLFW_KEY_D, InputKey::D},
        {GLFW_KEY_E, InputKey::E},
        {GLFW_KEY_F, InputKey::F},
        {GLFW_KEY_G, InputKey::G},
        {GLFW_KEY_H, InputKey::H},
        {GLFW_KEY_I, InputKey::I},
        {GLFW_KEY_J, InputKey::J},
        {GLFW_KEY_K, InputKey::K},
        {GLFW_KEY_L, InputKey::L},
        {GLFW_KEY_M, InputKey::M},
        {GLFW_KEY_N, InputKey::N},
        {GLFW_KEY_O, InputKey::O},
        {GLFW_KEY_P, InputKey::P},
        {GLFW_KEY_Q, InputKey::Q},
        {GLFW_KEY_R, InputKey::R},
        {GLFW_KEY_S, InputKey::S},
        {GLFW_KEY_T, InputKey::T},
        {GLFW_KEY_U, InputKey::U},
        {GLFW_KEY_V, InputKey::V},
        {GLFW_KEY_W, InputKey::W},
        {GLFW_KEY_X, InputKey::X},
        {GLFW_KEY_Y, InputKey::Y},
        {GLFW_KEY_Z, InputKey::Z},

        {GLFW_KEY_0, InputKey::D0},
        {GLFW_KEY_1, InputKey::D1},
        {GLFW_KEY_2, InputKey::D2},
        {GLFW_KEY_3, InputKey::D3},
        {GLFW_KEY_4, InputKey::D4},
        {GLFW_KEY_5, InputKey::D5},
        {GLFW_KEY_6, InputKey::D6},
        {GLFW_KEY_7, InputKey::D7},
        {GLFW_KEY_8, InputKey::D8},
        {GLFW_KEY_9, InputKey::D9},

        {GLFW_KEY_F1, InputKey::F1},
        {GLFW_KEY_F2, InputKey::F2},
        {GLFW_KEY_F3, InputKey::F3},
        {GLFW_KEY_F4, InputKey::F4},
        {GLFW_KEY_F5, InputKey::F5},
        {GLFW_KEY_F6, InputKey::F6},
        {GLFW_KEY_F7, InputKey::F7},
        {GLFW_KEY_F8, InputKey::F8},
        {GLFW_KEY_F9, InputKey::F9},
        {GLFW_KEY_F10, InputKey::F10},
        {GLFW_KEY_F11, InputKey::F11},
        {GLFW_KEY_F12, InputKey::F12},

        {GLFW_KEY_ESCAPE, InputKey::Escape},
        {GLFW_KEY_SPACE, InputKey::Space},
        {GLFW_KEY_TAB, InputKey::Tab},
        {GLFW_KEY_CAPS_LOCK, InputKey::CapsLock},
        {GLFW_KEY_ENTER, InputKey::Enter},
        {GLFW_KEY_BACKSPACE, InputKey::Backspace},
        {GLFW_KEY_INSERT, InputKey::Insert},
        {GLFW_KEY_DELETE, InputKey::Delete},
        {GLFW_KEY_HOME, InputKey::Home},
        {GLFW_KEY_END, InputKey::End},
        {GLFW_KEY_PAGE_UP, InputKey::PageUp},
        {GLFW_KEY_PAGE_DOWN, InputKey::PageDown},

        {GLFW_KEY_LEFT, InputKey::Left},
        {GLFW_KEY_RIGHT, InputKey::Right},
        {GLFW_KEY_UP, InputKey::Up},
        {GLFW_KEY_DOWN, InputKey::Down},

        {GLFW_KEY_LEFT_SHIFT, InputKey::Shift},
        {GLFW_KEY_LEFT_CONTROL, InputKey::Control},
        {GLFW_KEY_LEFT_ALT, InputKey::Alt},
        {GLFW_KEY_LEFT_SUPER, InputKey::Super},

        {GLFW_KEY_RIGHT_SHIFT, InputKey::Shift},
        {GLFW_KEY_RIGHT_CONTROL, InputKey::Control},
        {GLFW_KEY_RIGHT_ALT, InputKey::Alt},
        {GLFW_KEY_RIGHT_SUPER, InputKey::Super},

        {GLFW_KEY_PRINT_SCREEN, InputKey::PrintScreen},
        {GLFW_KEY_SCROLL_LOCK, InputKey::ScrollLock},
        {GLFW_KEY_PAUSE, InputKey::Pause},

        {GLFW_KEY_NUM_LOCK, InputKey::NumLock},
        {GLFW_KEY_KP_0, InputKey::NumPad0},
        {GLFW_KEY_KP_1, InputKey::NumPad1},
        {GLFW_KEY_KP_2, InputKey::NumPad2},
        {GLFW_KEY_KP_3, InputKey::NumPad3},
        {GLFW_KEY_KP_4, InputKey::NumPad4},
        {GLFW_KEY_KP_5, InputKey::NumPad5},
        {GLFW_KEY_KP_6, InputKey::NumPad6},
        {GLFW_KEY_KP_7, InputKey::NumPad7},
        {GLFW_KEY_KP_8, InputKey::NumPad8},
        {GLFW_KEY_KP_9, InputKey::NumPad9},
        {GLFW_KEY_KP_DECIMAL, InputKey::NumPadDecimal},
        {GLFW_KEY_KP_DIVIDE, InputKey::NumPadDivide},
        {GLFW_KEY_KP_MULTIPLY, InputKey::NumPadMultiply},
        {GLFW_KEY_KP_SUBTRACT, InputKey::NumPadSubtract},
        {GLFW_KEY_KP_ADD, InputKey::NumPadAdd},
        {GLFW_KEY_KP_ENTER, InputKey::NumPadEnter},
        {GLFW_KEY_KP_EQUAL, InputKey::NumPadEqual},

        {GLFW_KEY_SEMICOLON, InputKey::Semicolon},
        {GLFW_KEY_EQUAL, InputKey::Equal},
        {GLFW_KEY_COMMA, InputKey::Comma},
        {GLFW_KEY_MINUS, InputKey::Minus},
        {GLFW_KEY_PERIOD, InputKey::Period},
        {GLFW_KEY_SLASH, InputKey::Slash},
        {GLFW_KEY_GRAVE_ACCENT, InputKey::Backtick},
        {GLFW_KEY_LEFT_BRACKET, InputKey::LeftBracket},
        {GLFW_KEY_BACKSLASH, InputKey::Backslash},
        {GLFW_KEY_RIGHT_BRACKET, InputKey::RightBracket},
        {GLFW_KEY_APOSTROPHE, InputKey::Quote},

        {GLFW_MOUSE_BUTTON_LEFT, InputKey::MouseLeft},
        {GLFW_MOUSE_BUTTON_RIGHT, InputKey::MouseRight},
        {GLFW_MOUSE_BUTTON_MIDDLE, InputKey::MouseMiddle},
        {GLFW_MOUSE_BUTTON_4, InputKey::Mouse4},
        {GLFW_MOUSE_BUTTON_5, InputKey::Mouse5},
    };

    /**
     * @class InputListenGLF3System
     * @brief Реализация InputListen для платформы GLFW.
     *
     * Этот класс является конкретной реализацией абстрактного интерфейса InputListen.
     * Он подключается к окну GLFW, регистрирует колбэки и хранит состояние клавиш и мыши.
     * Предоставляет методы для опроса состояния ввода, а также транслирует события
     * через унаследованные делегаты (OnKeyPressed и т.д.).
     */

    class InputListenGLFWSystem : public IInputListen
    {
    public:

        InputListenGLFWSystem() = default;
        virtual ~InputListenGLFWSystem() override;

        // Переопределение виртуальных методов базового класса
        virtual void Init(class Window* InWindow) override;
        virtual void DeInit() override;

        // Управление курсором
        virtual bool GetCursorVisible();                    
        virtual int GetCursorMode();
        virtual void SetCursorVisible(bool Visible) override;                    
        virtual void SetCursorMode(int Mode) override; // 0=Normal, 1=Hidden, 2=Disable

        // Утилиты
        void* GetWindowHandle();                    // Получить нативный дескриптор окна
        InputKey InputKeyFromInt(int IntKey);       // Преобразовать целочисленный код в InputKey

        static InputListenGLFWSystem* GetInputListen(GLFWwindow* window);

    protected:

        void* m_windowHandle = nullptr;                     // Указатель на нативный дескриптор окна (GLFWwindow*)
        void* m_window = nullptr;
        DelegateHandle DelegateChangeWindowHandle;

        // Преобразования между InputKey и GLFW-кодами
        static InputKey FromGLFWKey(int glfwKey);
        static int ToGLFWMouseButton(InputKey button);
        // Статические функции-обработчики, которые регистрируются в GLFW как колбэки.
        static void OnGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void OnGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void OnGLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void OnGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        // Класс Window объявлен другом, чтобы иметь доступ к приватным членам
        friend class Window;

        // Преобразование кнопок в формат движка

        static InputKey GLFWKeyToEngineKey(int glfwKey) 
        {
            //return GLFWToEngineKey(glfwKey);
            auto it = GLFWInputToEngineMap.find(glfwKey);
                if (it != GLFWInputToEngineMap.end())
                    return it->second;
            return InputKey::Unknown;
        }
    };
}

#endif