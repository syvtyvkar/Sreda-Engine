// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Input/Input.h"                                 // Основной заголовок системы ввода
#include "Engine/Core/Log.h"                                    // Логирование

#ifdef ENGINE_GLFW
#include <GLFW/glfw3.h>                                         // Заголовок GLFW (платформозависимый)
#include "Engine/Input/InputCodes.h"                            // Таблицы соответствия клавиш (GLFW <-> Engine)
#include "Engine/Platform/WindowAPI/WindowGLF3/WindowGLF3.h"    // Класс окна GLFW
#endif

#include "Engine/Input/InputAPI/InputGLF3/InputGLF3.h"          // Специфичная для GLFW часть ввода

#include <unordered_map>                                        // Пока не используется

namespace Engine::Input 
{
    InputSystem* InputSystem::s_InputInstance = nullptr;        // Инициализация статического указателя на единственный экземпляр InputSystem.

    /*************** API */

#ifdef ENGINE_GLFW

    int InputSystem::ToGLFWKey(InputKey key)  // Преобразует внутренний код клавиши (InputKey) в соответствующий код GLFW.          
    {
        auto idx = static_cast<uint32_t>(key);
        if (idx >= sizeof(GLFW::KeyToGLFW) / sizeof(GLFW::KeyToGLFW[0])) 
        {
            return GLFW_KEY_UNKNOWN;  // Если индекс выходит за пределы таблицы
        }
        return GLFW::KeyToGLFW[idx];
    }

    InputKey InputSystem::FromGLFWKey(int glfwKey) {return GLFW::GLFWKeyToEngineKey(glfwKey);};             // Преобразует код клавиши GLFW во внутренний InputKey.

    int InputSystem::ToGLFWMouseButton(InputKey button) {return ToGLFWKey(button);};                        // Преобразует кнопку мыши (InputKey) в код GLFW (используется та же таблица, что и для клавиш, но с учётом сдвига).

    void InputSystem::OnGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)    // Колбэк, вызываемый GLFW при событии клавиатуры.
    {
        if (!s_InputInstance) return;
        
        InputKey engineKey = InputSystem::FromGLFWKey(key);                                 // Преобразуем GLFW-код клавиши во внутренний.
        if (engineKey == InputKey::Unknown) return;                                         // Игнорируем неизвестные клавиши

        InputState state = InputState::Unknown;                                             // Определяем тип действия (нажатие, повтор, отпускание).
        switch (action) {
            case GLFW_PRESS:    state = InputState::Pressed; break;
            case GLFW_REPEAT:   state = InputState::Repeated; break;
            case GLFW_RELEASE:  state = InputState::Released; break;
        }

        s_InputInstance->OnKeyPressed().Broadcast(key,mods,scancode);                       // Вызов делегата (события) нажатия клавиши.

        if (state != InputState::Unknown) s_InputInstance->SetKeyState(engineKey, state);   // Если состояние определено, обновляем внутреннее состояние клавиши.
    }

    void InputSystem::OnGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)   // Колбэк для событий кнопок мыши.
    {
        if (!s_InputInstance) return;
        
        // Кнопки мыши в GLFW имеют коды, начиная с 0, но мы сдвигаем их на 1000, чтобы отличать от клавиш клавиатуры
        InputKey engineButton = FromGLFWKey(button + 1000);  // Сдвиг для мыши
        if (engineButton == InputKey::Unknown) return;

        InputState state = (action == GLFW_PRESS) ? InputState::Pressed : (action == GLFW_RELEASE) ? InputState::Released : InputState::Unknown;    // Определяем действие (нажатие/отпускание).

        // Вызов соответствующих делегатов.
        if (state == InputState::Pressed)
        {
            s_InputInstance->OnMouseButtonPressed().Broadcast(button,mods);
        }else if (state == InputState::Released)
        {
            s_InputInstance->OnMouseButtonReleased().Broadcast(button,mods);
        }
        

        if (state != InputState::Unknown) s_InputInstance->SetKeyState(engineButton, state);    // Обновляем состояние кнопки, если оно определено.

    }

    void InputSystem::OnGLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos)     // Колбэк перемещения курсора мыши.
    {
        if (!s_InputInstance) return;
        
        glm::vec2 newPos = {static_cast<float>(xpos), static_cast<float>(ypos)};
        s_InputInstance->m_mouseDelta = newPos - s_InputInstance->m_mousePosition;              // Вычисляем смещение
        s_InputInstance->m_mousePosition = newPos;                                              // Обновляем текущую позицию
    }

    void InputSystem::OnGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset)  // Колбэк прокрутки колесика мыши.
    {
        if (!s_InputInstance) return;
        s_InputInstance->m_scrollDelta = yoffset;  // Обычно используем только вертикальный скролл
    }

#endif

    /*************** BASE (базовые методы инициализации/завершения) */

    void InputSystem::Init(Engine::Window* windowHandle)                    // Инициализация системы ввода.
    {
        if (s_InputInstance) {
            ENGINE_LOG_WARN("Input already initialized!");                  // Предупреждение о повторной инициализации
            return;
        }
        s_InputInstance = new InputSystem();
        s_InputInstance->m_windowHandle=windowHandle;                       // Сохраняем указатель на окно

        // Обнуляем массивы состояний клавиш (текущее и предыдущее).
        for (auto& LState : s_InputInstance->m_keyStates) LState = 0;
        for (auto& LState : s_InputInstance->m_prevKeyStates) LState = 0;

        #ifdef ENGINE_GLFW
        // Приведение указателя к конкретному типу окна GLFW.
        Engine::WindowGLF3* WindowGL = static_cast<Engine::WindowGLF3*>(windowHandle);
        auto* glfwWindow = static_cast<GLFWwindow*>(WindowGL->GetHandle());

         // Устанавливаем колбэки GLFW.
        glfwSetKeyCallback(glfwWindow, OnGLFWKeyCallback);
        glfwSetMouseButtonCallback(glfwWindow, OnGLFWMouseButtonCallback);
        glfwSetCursorPosCallback(glfwWindow, OnGLFWCursorPosCallback);
        glfwSetScrollCallback(glfwWindow, OnGLFWScrollCallback);
        #endif
        ENGINE_LOG_TRACE("Input system start!");
    }

    void InputSystem::Shutdown()                        // Завершение работы системы ввода.
    {
        if (!s_InputInstance) return;

        #ifdef ENGINE_GLFW
        // Сбрасываем колбэки GLFW, чтобы они не вызывались после удаления InputSystem.
        auto* glfwWindow = static_cast<GLFWwindow*>(s_InputInstance->m_windowHandle);
        if (glfwWindow)
        {
            glfwSetKeyCallback(glfwWindow, nullptr);
            glfwSetMouseButtonCallback(glfwWindow, nullptr);
            glfwSetCursorPosCallback(glfwWindow, nullptr);
            glfwSetScrollCallback(glfwWindow, nullptr);
        }
        #endif
        delete s_InputInstance;
        s_InputInstance = nullptr;
        ENGINE_LOG_INFO("Input system shutdown");
    }

    void InputSystem::Update()                          // Обновление состояния ввода (вызывается каждый кадр).
    {
        if (!s_InputInstance) return;

        // Сохраняем текущие состояния как "предыдущие" для следующего кадра.
        memcpy(s_InputInstance->m_prevKeyStates, s_InputInstance->m_keyStates, sizeof(s_InputInstance->m_keyStates));
        // Сбрасываем дельту мыши и скролл, так как они накапливаются только за один кадр.
        s_InputInstance->m_mouseDelta={0,0};
        s_InputInstance->m_scrollDelta=0.0;
    }

    /*************** INPUTS (опрос состояния клавиш) */

    bool InputSystem::IsKeyPressed(InputKey Key)    // Проверка, зажата ли клавиша в текущем кадре (включая повтор).
    {
        if (!s_InputInstance) return false;
        return s_InputInstance->GetKeyStateInternal(Key) == InputState::Pressed || s_InputInstance->GetKeyStateInternal(Key) == InputState::Repeated;
    }

    bool InputSystem::IsKeyJustPressed(InputKey Key)    // Проверка, было ли только что (в этом кадре) нажатие клавиши.
    {
        if (!s_InputInstance) return false;
        return s_InputInstance->GetKeyStateInternal(Key) == InputState::Pressed && s_InputInstance->m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }

    bool InputSystem::IsKeyJustReleased(InputKey Key)   // Проверка, было ли только что отпускание клавиши.
    {
        if (!s_InputInstance) return false;
        return s_InputInstance->GetKeyStateInternal(Key) == InputState::Released || s_InputInstance->m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }


    InputState InputSystem::GetKeyState(InputKey Key)   // Получение текущего состояния клавиши.
    {
        if (!s_InputInstance) return InputState::Unknown;
        return s_InputInstance->GetKeyStateInternal(Key);
    }

    /*************** MOUSE (опрос мыши) */

    bool InputSystem::IsMouseButtonPressed(InputKey Button) {return IsKeyPressed(Button);};             // Проверка, зажата ли кнопка мыши (использует тот же механизм, что и клавиши).
    bool InputSystem::IsMouseButtonJustPressed(InputKey Button) {return IsKeyJustPressed(Button);};     // Проверка, была ли только что нажата кнопка мыши.

    glm::vec2 InputSystem::GetMousePosition()   // Возвращает текущую позицию курсора.
    {
        if (!s_InputInstance) return {0,0};
        return s_InputInstance->m_mousePosition;
    };

    glm::vec2 InputSystem::GetMouseDelta()      // Возвращает изменение позиции курсора за последний кадр.
    {
        if (!s_InputInstance) return {0,0};
        return s_InputInstance->m_mouseDelta;
    }

    double InputSystem::GetMouseScrollDelta()   // Возвращает величину прокрутки колесика за последний кадр.
    {
        if (!s_InputInstance) return 0,0;
        return s_InputInstance->m_scrollDelta;
    }

    /*************** CURSOR (управление курсором) */

    void InputSystem::SetCursorVisible(bool InVisible)  // Установка видимости курсора.
    {
        #ifdef ENGINE_GLFW
        if (s_InputInstance && s_InputInstance->m_windowHandle)
        {
            auto* glfwWindow = static_cast<GLFWwindow*>(s_InputInstance->m_windowHandle);
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, InVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
        }
        #endif
    }

    void InputSystem::SetCursorMode(int mode)   // Установка режима курсора: 0 - нормальный, 1 - скрытый, 2 - захваченный (disabled).
    {
        #ifdef ENGINE_GLFW
        if (s_InputInstance && s_InputInstance->m_windowHandle)
        {
            auto* glfwWindow = static_cast<GLFWwindow*>(s_InputInstance->m_windowHandle);
            int glfwMode;
            switch (mode)
            {
                case 1:
                glfwMode = GLFW_CURSOR_HIDDEN;
                break;
                case 2:
                glfwMode = GLFW_CURSOR_DISABLED;
                break;
            default:
                glfwMode = GLFW_CURSOR_NORMAL;
                break;
            }
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, glfwMode);
        }
        #endif
    }

    /*************** METODS (прочие методы) */

    InputSystem& InputSystem::GetInstance() // Возвращает ссылку на единственный экземпляр.
    {
        return *s_InputInstance;
    }

    void* InputSystem::GetWindowHandle()        // Возвращает нативный дескриптор окна (платформозависимый).
    {
        return s_InputInstance ? s_InputInstance->m_windowHandle : nullptr;
    }

    InputKey InputSystem::InputKeyFromInt(int IntKey)   // Преобразует целочисленный код клавиши (например, из GLFW) в InputKey.
    {
#ifdef ENGINE_GLFW
    return FromGLFWKey(IntKey);
#endif
    return InputKey::Unknown;
    };

    void InputSystem::SetKeyState(InputKey Key, InputState State)   // Устанавливает состояние конкретной клавиши (внутренний метод).
    {
        auto idx = static_cast<uint32_t>(Key);
        if (idx < static_cast<uint32_t>(InputKey::KeyCount))
        {
            m_keyStates[idx] = static_cast<uint8_t>(State);
        }
    }

    InputState InputSystem::GetKeyStateInternal(InputKey Key) const // Внутренний метод получения состояния клавиши.
    {
        auto idx = static_cast<uint32_t>(Key);
        if (idx >= static_cast<uint32_t>(InputKey::KeyCount))
        {
            return InputState::Unknown;
        }
        return static_cast<InputState>(m_keyStates[idx]);
    }
}