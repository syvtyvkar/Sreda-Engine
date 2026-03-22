// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Input/Input.h"                                 // Основной заголовок системы ввода
#include "Engine/Core/Log.h"                                    // Логирование
#include "Engine/Core/Time.h"    
#include "Input.h"

namespace Engine
{
    InputSystem* InputSystem::s_InputInstance = nullptr;        // Инициализация статического указателя на единственный экземпляр InputSystem.

    /*************** BASE (базовые методы инициализации/завершения) */

    void InputSystem::Init(Engine::Window* windowHandle)                    // Инициализация системы ввода.
    {
        if (s_InputInstance) {
            ENGINE_LOG_WARN("Input already initialized!");                  // Предупреждение о повторной инициализации
            return;
        }
        s_InputInstance = new InputSystem();

        // Обнуляем массивы состояний клавиш (текущее и предыдущее).
        for (auto& LState : s_InputInstance->m_keyStates) LState = 0;
        for (auto& LState : s_InputInstance->m_prevKeyStates) LState = 0;

        s_InputInstance->m_InputListen = InputListenAPIFactory::create();
        if (!s_InputInstance->m_InputListen.get())
        {
            ENGINE_LOG_ERROR("ERROR SERACH INPUT LISTEN!");
            return;
        }
        s_InputInstance->m_InputListen.get()->Init(windowHandle);
        s_InputInstance->m_InputListen.get()->OnKeyPressed().Subscribe([&](InputKey key, int mods, int repeat,InputState state) { s_InputInstance->CallOnKeyPressed(key,mods,repeat,state);});
        s_InputInstance->m_InputListen.get()->OnMouseButtonPressed().Subscribe([&](InputKey button, int mods,InputState state) {s_InputInstance->CallOnMouseButtonPressed(button,mods,state);});
        s_InputInstance->m_InputListen.get()->OnMouseMoved().Subscribe([&](float x, float y) {s_InputInstance->CallOnMouseMoved(x,y);});
        s_InputInstance->m_InputListen.get()->OnMouseScrolled().Subscribe([&](float x, float y) {s_InputInstance->CallOnMouseScrolled(x,y);});

        ENGINE_LOG_TRACE("Input system start!");
    }

    void InputSystem::Shutdown()                        // Завершение работы системы ввода.
    {
        if (!s_InputInstance) return;

        s_InputInstance->m_InputListen.get()->DeInit();
        s_InputInstance->m_InputListen.reset();

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
        if (!s_InputInstance->GetInputListen()) return false;
        return s_InputInstance->GetKeyStateInternal(Key) == InputState::Pressed || s_InputInstance->GetKeyStateInternal(Key) == InputState::Repeated;
    }

    bool InputSystem::IsKeyJustPressed(InputKey Key)    // Проверка, было ли только что (в этом кадре) нажатие клавиши.
    {
        if (!s_InputInstance) return false;
        if (!s_InputInstance->GetInputListen()) return false;
        return s_InputInstance->GetKeyStateInternal(Key) == InputState::Pressed && s_InputInstance->m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }

    bool InputSystem::IsKeyJustReleased(InputKey Key)   // Проверка, было ли только что отпускание клавиши.
    {
        if (!s_InputInstance) return false;
        if (!s_InputInstance->GetInputListen()) return false;
        return s_InputInstance->GetKeyStateInternal(Key) == InputState::Released || s_InputInstance->m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }


    InputState InputSystem::GetKeyState(InputKey Key)   // Получение текущего состояния клавиши.
    {
        if (!s_InputInstance) return InputState::Unknown;
        if (!s_InputInstance->GetInputListen()) InputState::Unknown;
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

    float InputSystem::GetMouseSensivity()
    {
        if (!s_InputInstance) return 0,0;
        return s_InputInstance->m_mouseSensivity;
    }

    bool InputSystem::IsInverMoveY()
    {
        if (!s_InputInstance) return false;
        return s_InputInstance->m_Invers_Y;
    }

    /*************** CURSOR (управление курсором) */

    bool InputSystem::GetCursorVisible()
    {
        if (s_InputInstance && s_InputInstance->GetInputListen())
        {
            return s_InputInstance->GetInputListen()->GetCursorVisible();
        }
        ENGINE_LOG_WARN("No search InputListen!");
        return false;
    }

    int InputSystem::GetCursorMode()
    {
        if (s_InputInstance && s_InputInstance->GetInputListen())
        {
            return s_InputInstance->GetInputListen()->GetCursorMode();
        }
        ENGINE_LOG_WARN("No search InputListen!");
        return 0;
    }

    void InputSystem::SetCursorVisible(bool InVisible) // Установка видимости курсора.
    {
        if (s_InputInstance && s_InputInstance->GetInputListen())
        {
            s_InputInstance->GetInputListen()->SetCursorVisible(InVisible);
        }
    }

    void InputSystem::SetCursorMode(int mode)   // Установка режима курсора: 0 - нормальный, 1 - скрытый, 2 - захваченный (disabled).
    {
        if (s_InputInstance && s_InputInstance->GetInputListen())
        {
            s_InputInstance->GetInputListen()->SetCursorMode(mode);
        }
    }

    /*************** METODS (прочие методы) */

    InputSystem& InputSystem::GetInstance() // Возвращает ссылку на единственный экземпляр.
    {
        return *s_InputInstance;
    }

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

void Engine::InputSystem::CallOnKeyPressed(InputKey key, int scancode, int mods, InputState State)
{
    if (!s_InputInstance) return;
    s_InputInstance->SetKeyState(key, State);
    s_InputInstance->OnKeyPressed().Broadcast(key, scancode, mods);
}

void Engine::InputSystem::CallOnMouseMoved(float x, float y)
{
    if (!s_InputInstance) return;
    float LNewX = x-GetMousePosition().x;
    float LNewY = y-GetMousePosition().y;
    LNewY = IsInverMoveY() ? LNewY*-1.f : LNewY;
    //float LDeltaTime = Engine::Time::TimeSystem::GetDeltaTime();
    //float LSensivityMouse = GetMouseSensivity();
    ENGINE_LOG_TRACE("Mouse Move: x {} y {}",LNewX,LNewY);
    s_InputInstance->OnMouseMoved().Broadcast(LNewX, LNewY);
    s_InputInstance->m_mousePosition.x=x;
    s_InputInstance->m_mousePosition.y=y;
}

void Engine::InputSystem::CallOnMouseScrolled(float x, float y)
{
    if (!s_InputInstance) return;
     s_InputInstance->OnMouseScrolled().Broadcast(x, y);
}

void Engine::InputSystem::CallOnMouseButtonPressed(InputKey button, int mods, InputState State)
{
    if (!s_InputInstance) return;
    s_InputInstance->SetKeyState(button, State);
    s_InputInstance->OnMouseButtonPressed().Broadcast(button, mods);
}