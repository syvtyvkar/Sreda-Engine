// (c) Nikita Rogalev. All rights reserved.

#ifdef ENGINE_WINDOW_GLFW

#include "InputGLFW.h"                                              // Заголовок класса 
#include "Engine/Core/Log.h"                                        // Логирование
#include "Engine/Input/Input.h"                                     // TODO: убрать!!! Основной класс InputSystem (для GetInstance) 

#include <GLFW/glfw3.h>                                             // Заголовок GLFW (колбэки, константы)
#include "Engine/Platform/IWindow.h"        
#include "Engine/Platform/WindowAPI/WindowGLFW/WindowGLFW.h"        // Класс окна GLFW (для получения native handle)

#include <unordered_map>                                            // TODO: проверить и убрать, возможно, ныне не требуется

namespace Engine 
{
    /**
     * @brief Инициализация слушателя ввода для GLFW.
     */
    void InputListenGLFWSystem::Init(class Window* InWindow)
    {
        if (!InWindow) 
        {
            ENGINE_LOG_ERROR("ERROR! Window NULL!");
            return;
        }
        // Приведение к конкретному типу окна GLFW.
        Engine::WindowGLFW* WindowGL = reinterpret_cast<WindowGLFW*>(InWindow);
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
    void InputListenGLFWSystem::DeInit()
    {
        OnKeyPressed().Clear();
        OnMouseButtonPressed().Clear();
        OnMouseMoved().Clear();
        OnMouseScrolled().Clear();

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
     * @brief Преобразует код клавиши GLFW во внутренний InputKey.
     * @param glfwKey Код клавиши из GLFW.
     * @return Соответствующий InputKey.
     */
    InputKey InputListenGLFWSystem::FromGLFWKey(int glfwKey) {return GLFWKeyToEngineKey(glfwKey);};

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
    void InputListenGLFWSystem::OnGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
    {
        if (!GetInputListen(window)) return;

        InputKey engineKey = InputListenGLFWSystem::FromGLFWKey(key);
        if (engineKey == InputKey::Unknown) return;

        InputState state = InputState::Unknown;
        switch (action) {
            case GLFW_PRESS:    state = InputState::Pressed; break;
            case GLFW_REPEAT:   state = InputState::Repeated; break;
            case GLFW_RELEASE:  state = InputState::Released; break;
        }

        GetInputListen(window)->OnKeyPressed().Broadcast(engineKey,mods,scancode, state);
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
    void InputListenGLFWSystem::OnGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
    {
        if (!GetInputListen(window)) return;
        InputKey engineButton = FromGLFWKey(button + 1000);  // Сдвиг для мыши
        if (engineButton == InputKey::Unknown) return;

        InputState state = (action == GLFW_PRESS) ? InputState::Pressed : (action == GLFW_RELEASE) ? InputState::Released : InputState::Unknown;

        GetInputListen(window)->OnMouseButtonPressed().Broadcast(engineButton,mods,state);
    }

    /**
     * @brief Статический колбэк GLFW для перемещения курсора мыши.
     * @param window Указатель на окно GLFW.
     * @param xpos Новая координата X (в пикселях окна).
     * @param ypos Новая координата Y.
     *
     * Вычисляет дельту перемещения с предыдущей позиции и сохраняет новую позицию.
     */
    void InputListenGLFWSystem::OnGLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
    {
        if (!GetInputListen(window)) return;

        GetInputListen(window)->OnMouseMoved().Broadcast(xpos,ypos);
    }

    /**
     * @brief Статический колбэк GLFW для прокрутки колесика мыши.
     * @param window Указатель на окно GLFW.
     * @param xoffset Горизонтальная прокрутка.
     * @param yoffset Вертикальная прокрутка.
     *
     * Сохраняет вертикальную прокрутку (горизонтальная игнорируется).
     */
    void InputListenGLFWSystem::OnGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) 
    {
        if (!GetInputListen(window)) return;

        GetInputListen(window)->OnMouseScrolled().Broadcast(xoffset,yoffset);
    }

    bool InputListenGLFWSystem::GetCursorVisible()
    {
        if (m_windowHandle)
        {
            if (glfwGetInputMode(static_cast<GLFWwindow*>(m_windowHandle),GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
            {
                return true;
            }else if (glfwGetInputMode(static_cast<GLFWwindow*>(m_windowHandle),GLFW_CURSOR) == GLFW_CURSOR_HIDDEN)
            {
                return false;
            }

        }
        return false;
    }

    int InputListenGLFWSystem::GetCursorMode()
    {
        if (m_windowHandle)
        {
            return glfwGetInputMode(static_cast<GLFWwindow*>(m_windowHandle),GLFW_CURSOR);
        }
        return 0;
    }

    void InputListenGLFWSystem::SetCursorVisible(bool InVisible)
    {
        if (m_windowHandle)
        {
            auto* glfwWindow = static_cast<GLFWwindow*>(m_windowHandle);
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, InVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
    }

    void InputListenGLFWSystem::SetCursorMode(int mode)
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

    void* InputListenGLFWSystem::GetWindowHandle()
    {
        return m_windowHandle;
    }

    InputKey InputListenGLFWSystem::InputKeyFromInt(int IntKey)
    {
        return FromGLFWKey(IntKey);
    }

    InputListenGLFWSystem *InputListenGLFWSystem::GetInputListen(GLFWwindow *window)
    {
        if (!Engine::InputSystem::GetInstance().GetInputListen()) return nullptr;
        return static_cast<InputListenGLFWSystem*>(Engine::InputSystem::GetInstance().GetInputListen());
    };

    InputListenGLFWSystem::~InputListenGLFWSystem() // Деструктор: автоматически вызывает DeInit для очистки.
    {
        DeInit();
    }
}

#endif