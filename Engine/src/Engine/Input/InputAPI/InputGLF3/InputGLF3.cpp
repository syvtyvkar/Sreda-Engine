// (c) Nikita Rogalev. All rights reserved.

#include "InputGLF3.h"                                              // Заголовок класса 
#include "Engine/Core/Log.h"                                        // Логирование
#include "Engine/Input/Input.h"                                     // TODO: убрать!!! Основной класс InputSystem (для GetInstance) 

#include <GLFW/glfw3.h>                                             // Заголовок GLFW (колбэки, константы)
#include "Engine/Input/InputCodes.h"                                // Таблицы соответствия клавиш (GLFW <-> Engine)
#include "Engine/Platform/WindowAPI/WindowGLF3/WindowGLF3.h"        // Класс окна GLFW (для получения native handle)

#include <unordered_map>                                            // TODO: проверить и убрать, возможно, ныне не требуется

using namespace Engine::Input;

namespace Engine::Input 
{
    /**
     * @brief Инициализация слушателя ввода для GLFW.
     */
    void InputListenGLF3System::Init(InputSystem *InInput)
    {
        if (!InInput) 
        {
            ENGINE_LOG_WARN("Input already initialized!");
            return;
        }
        // Приведение к конкретному типу окна GLFW.
        Engine::WindowGLF3* WindowGL = static_cast<Engine::WindowGLF3*>(InInput->GetWindowHandle());
        auto* glfwWindow = static_cast<GLFWwindow*>(WindowGL->GetHandle());

        // Установка колбэков GLFW на это окно.
        glfwSetKeyCallback(glfwWindow, OnGLFWKeyCallback);
        glfwSetMouseButtonCallback(glfwWindow, OnGLFWMouseButtonCallback);
        glfwSetCursorPosCallback(glfwWindow, OnGLFWCursorPosCallback);
        glfwSetScrollCallback(glfwWindow, OnGLFWScrollCallback);

        m_windowHandle = glfwWindow;                                    // Сохраняем native handle для дальнейшего использования
        ENGINE_LOG_TRACE("Input listen start!");
    }

    /**
     * @brief Деинициализация слушателя: сбрасывает колбэки GLFW и очищает handle.
     */
    void InputListenGLF3System::DeInit()
    {
        auto* glfwWindow = static_cast<GLFWwindow*>(m_windowHandle);
        if (glfwWindow)
        {
            // Убираем колбэки, чтобы они не вызывались после удаления объекта.
            glfwSetKeyCallback(glfwWindow, nullptr);
            glfwSetMouseButtonCallback(glfwWindow, nullptr);
            glfwSetCursorPosCallback(glfwWindow, nullptr);
            glfwSetScrollCallback(glfwWindow, nullptr);
        }
        m_windowHandle = nullptr; 
        ENGINE_LOG_INFO("Input listen shutdown");
    }

    /**
     * @brief Преобразует внутренний код клавиши (InputKey) в соответствующий код GLFW.
     * @param key Внутренний код клавиши.
     * @return Код GLFW или GLFW_KEY_UNKNOWN, если преобразование невозможно.
     */
    int InputListenGLF3System::ToGLFWKey(InputKey key) 
    {
        auto idx = static_cast<uint32_t>(key);
        if (idx >= sizeof(GLFW::KeyToGLFW) / sizeof(GLFW::KeyToGLFW[0])) 
        {
            return GLFW_KEY_UNKNOWN;
        }
        return GLFW::KeyToGLFW[idx];
    }

    /**
     * @brief Преобразует код клавиши GLFW во внутренний InputKey.
     * @param glfwKey Код клавиши из GLFW.
     * @return Соответствующий InputKey.
     */
    InputKey InputListenGLF3System::FromGLFWKey(int glfwKey) {return GLFW::GLFWKeyToEngineKey(glfwKey);};

    /**
     * @brief Преобразует кнопку мыши (InputKey) в код GLFW.
     *        Используется та же таблица, что и для клавиш, но с учётом сдвига (в колбэках).
     * @param button Внутренний код кнопки мыши.
     * @return Код GLFW для кнопки мыши.
     */
    int InputListenGLF3System::ToGLFWMouseButton(InputKey button) {return ToGLFWKey(button);};

    /**
     * @brief Статический колбэк GLFW для событий клавиатуры.
     * @param window Указатель на окно GLFW.
     * @param key Код клавиши GLFW.
     * @param scancode Скан-код (аппаратный).
     * @param action Действие: GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT.
     * @param mods Битовые флаги модификаторов (Ctrl, Shift и т.д.).
     *
     * Преобразует ключ во внутренний InputKey, определяет состояние (Pressed/Released/Repeated),
     * вызывает соответствующий делегат OnKeyPressed и обновляет внутреннее состояние клавиши.
     */
    void InputListenGLF3System::OnGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
    {
        InputKey engineKey = InputListenGLF3System::FromGLFWKey(key);
        if (engineKey == InputKey::Unknown) return;

        InputState state = InputState::Unknown;
        switch (action) {
            case GLFW_PRESS:    state = InputState::Pressed; break;
            case GLFW_REPEAT:   state = InputState::Repeated; break;
            case GLFW_RELEASE:  state = InputState::Released; break;
        }

        InputListenGLF3System* LInputListen = static_cast<InputListenGLF3System*>(Engine::Input::InputSystem::GetInstance().GetInputListen());
        LInputListen->OnKeyPressed().Broadcast(key,mods,scancode);

        if (state != InputState::Unknown) LInputListen->SetKeyState(engineKey, state);
    }

    /**
     * @brief Статический колбэк GLFW для событий кнопок мыши.
     * @param window Указатель на окно GLFW.
     * @param button Код кнопки мыши GLFW.
     * @param action Действие: GLFW_PRESS или GLFW_RELEASE.
     * @param mods Битовые флаги модификаторов.
     *
     * Код кнопки мыши сдвигается на 1000, чтобы отличать от клавиш клавиатуры.
     * Вызывает соответствующие делегаты OnMouseButtonPressed/Released и обновляет состояние.
     */
    void InputListenGLF3System::OnGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
    {
        InputKey engineButton = FromGLFWKey(button + 1000);  // Сдвиг для мыши
        if (engineButton == InputKey::Unknown) return;

        InputState state = (action == GLFW_PRESS) ? InputState::Pressed : (action == GLFW_RELEASE) ? InputState::Released : InputState::Unknown;

        InputListenGLF3System* LInputListen = static_cast<InputListenGLF3System*>(Engine::Input::InputSystem::GetInstance().GetInputListen());

        if (state == InputState::Pressed)
        {
            LInputListen->OnMouseButtonPressed().Broadcast(button,mods);
        }else if (state == InputState::Released)
        {
            LInputListen->OnMouseButtonReleased().Broadcast(button,mods);
        }
        
        if (state != InputState::Unknown) LInputListen->SetKeyState(engineButton, state);
    }

    /**
     * @brief Статический колбэк GLFW для перемещения курсора мыши.
     * @param window Указатель на окно GLFW.
     * @param xpos Новая координата X (в пикселях окна).
     * @param ypos Новая координата Y.
     *
     * Вычисляет дельту перемещения с предыдущей позиции и сохраняет новую позицию.
     */
    void InputListenGLF3System::OnGLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
    {
        InputListenGLF3System* LInputListen = static_cast<InputListenGLF3System*>(Engine::Input::InputSystem::GetInstance().GetInputListen());

        glm::vec2 newPos = {static_cast<float>(xpos), static_cast<float>(ypos)};
        LInputListen->m_mouseDelta = newPos - LInputListen->m_mousePosition;
        LInputListen->m_mousePosition = newPos;
    }

    /**
     * @brief Статический колбэк GLFW для прокрутки колесика мыши.
     * @param window Указатель на окно GLFW.
     * @param xoffset Горизонтальная прокрутка.
     * @param yoffset Вертикальная прокрутка.
     *
     * Сохраняет вертикальную прокрутку (горизонтальная игнорируется).
     */
    void InputListenGLF3System::OnGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) 
    {
        InputListenGLF3System* LInputListen = static_cast<InputListenGLF3System*>(Engine::Input::InputSystem::GetInstance().GetInputListen());

        LInputListen->m_scrollDelta = yoffset;  // Обычно используем только вертикальный скролл
    }

    bool InputListenGLF3System::IsKeyPressed(InputKey Key)
    {
        return GetKeyStateInternal(Key) == InputState::Pressed || GetKeyStateInternal(Key) == InputState::Repeated;
    }

    bool InputListenGLF3System::IsKeyJustPressed(InputKey Key)
    {
        return GetKeyStateInternal(Key) == InputState::Pressed && m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }

    bool InputListenGLF3System::IsKeyJustReleased(InputKey Key)
    {
        return GetKeyStateInternal(Key) == InputState::Released || m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }


    InputState InputListenGLF3System::GetKeyState(InputKey Key)
    {
        return GetKeyStateInternal(Key);
    }

    bool InputListenGLF3System::IsMouseButtonPressed(InputKey Button) {return IsKeyPressed(Button);};
    bool InputListenGLF3System::IsMouseButtonJustPressed(InputKey Button) {return IsKeyJustPressed(Button);};

    glm::vec2 InputListenGLF3System::GetMousePosition() 
    {
        return m_mousePosition;
    };

    glm::vec2 InputListenGLF3System::GetMouseDelta() 
    {
        return m_mouseDelta;
    }

    double InputListenGLF3System::GetMouseScrollDelta()
    {
        return m_scrollDelta;
    }

    void InputListenGLF3System::SetCursorVisible(bool InVisible)
    {
        #ifdef ENGINE_GLFW
        if (m_windowHandle)
        {
            auto* glfwWindow = static_cast<GLFWwindow*>(m_windowHandle);
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, InVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
        }
        #endif
    }

    void InputListenGLF3System::SetCursorMode(int mode)
    {
        if (m_windowHandle)
        {
            auto* glfwWindow = static_cast<GLFWwindow*>(m_windowHandle);
            int glfwMode;
            switch (mode)
            {
                case 1:
                glfwMode = GLFW_CURSOR_HIDDEN;                          // Скрытый, но не захваченный
                break;
                case 2:
                glfwMode = GLFW_CURSOR_DISABLED;                        // Захваченный (Как аналог GameInputMode в UE)
                break;
            default:
                glfwMode = GLFW_CURSOR_NORMAL;                          // Обычный режим
                break;
            }
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, glfwMode);
        }
    }

    void* InputListenGLF3System::GetWindowHandle()
    {
        return m_windowHandle;
    }

    InputKey InputListenGLF3System::InputKeyFromInt(int IntKey)
    {
#ifdef ENGINE_GLFW
    return FromGLFWKey(IntKey);
#endif
    return InputKey::Unknown;
    };

    InputListenGLF3System::~InputListenGLF3System() // Деструктор: автоматически вызывает DeInit для очистки.
    {
        DeInit();
    }

    void InputListenGLF3System::SetKeyState(InputKey Key, InputState State)
    {
        auto idx = static_cast<uint32_t>(Key);
        if (idx < static_cast<uint32_t>(InputKey::KeyCount))
        {
            m_keyStates[idx] = static_cast<uint8_t>(State);
        }
    }

    InputState InputListenGLF3System::GetKeyStateInternal(InputKey Key) const
    {
        auto idx = static_cast<uint32_t>(Key);
        if (idx >= static_cast<uint32_t>(InputKey::KeyCount))
        {
            return InputState::Unknown;
        }
        return static_cast<InputState>(m_keyStates[idx]);
    }
}