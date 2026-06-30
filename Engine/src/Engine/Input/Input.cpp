// (c) Nikita Rogalev. All rights reserved.

#include "Input.h"                                 // Main input system header

namespace Engine
{
    InputSystem* InputSystem::s_InputInstance = nullptr;        // Static pointer initialization for the single InputSystem instance.

    /*************** BASE (basic init/shutdown methods) */

    void InputSystem::Init(Engine::Window* windowHandle)                    // Initialize input system.
    {
        if (s_InputInstance) {
            ENGINE_LOG_WARN("Input already initialized!");                  // Warning about reinitialization
            return;
        }
        s_InputInstance = new InputSystem();

        // Zero out key state arrays (current and previous).
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
        s_InputInstance->m_InputListen.get()->OnCharInput().Subscribe([&](uint32_t codepoint) {s_InputInstance->OnCharInput().Broadcast(codepoint);});

        ENGINE_LOG_TRACE("Input system start!");
    }

    void InputSystem::Shutdown()                        // Shutdown input system.
    {
        if (!s_InputInstance) return;

        //s_InputInstance->m_InputListen.get()->DeInit();
        s_InputInstance->m_InputListen.reset();

        delete s_InputInstance;
        s_InputInstance = nullptr;
        ENGINE_LOG_INFO("Input system shutdown");
    }

    void InputSystem::Update()                          // Update input state (called every frame).
    {
        if (!s_InputInstance) return;

        // Save current states as "previous" for the next frame.
        memcpy(s_InputInstance->m_prevKeyStates, s_InputInstance->m_keyStates, sizeof(s_InputInstance->m_keyStates));
        // Reset mouse delta and scroll, as they accumulate only for one frame.
        s_InputInstance->m_mouseDelta={0,0};
        s_InputInstance->m_scrollDelta=0.0;
    }

    /*************** INPUTS (key state polling) */

    bool InputSystem::IsKeyPressed(InputKey Key)    // Check if a key is held down in the current frame (including repeat).
    {
        if (!s_InputInstance) return false;
        if (!s_InputInstance->GetInputListen()) return false;
        return s_InputInstance->GetKeyStateInternal(Key) == InputState::Pressed || s_InputInstance->GetKeyStateInternal(Key) == InputState::Repeated;
    }

    bool InputSystem::IsKeyJustPressed(InputKey Key)    // Check if a key was just pressed (this frame).
    {
        if (!s_InputInstance) return false;
        if (!s_InputInstance->GetInputListen()) return false;
        return s_InputInstance->GetKeyStateInternal(Key) == InputState::Pressed && s_InputInstance->m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }

    bool InputSystem::IsKeyJustReleased(InputKey Key)   // Check if a key was just released.
    {
        if (!s_InputInstance) return false;
        if (!s_InputInstance->GetInputListen()) return false;
        return s_InputInstance->GetKeyStateInternal(Key) == InputState::Released || s_InputInstance->m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }


    InputState InputSystem::GetKeyState(InputKey Key)   // Get current key state.
    {
        if (!s_InputInstance) return InputState::Unknown;
        if (!s_InputInstance->GetInputListen()) InputState::Unknown;
        return s_InputInstance->GetKeyStateInternal(Key);
    }

    /*************** MOUSE (mouse polling) */

    bool InputSystem::IsMouseButtonPressed(InputKey Button) {return IsKeyPressed(Button);};             // Check if a mouse button is held (uses same mechanism as keys).
    bool InputSystem::IsMouseButtonJustPressed(InputKey Button) {return IsKeyJustPressed(Button);};     // Check if a mouse button was just pressed.

    Vector2 InputSystem::GetMousePosition()   // Returns current cursor position.
    {
        if (!s_InputInstance) return {0,0};
        return s_InputInstance->m_mousePosition;
    };

    Vector2 InputSystem::GetMouseDelta()      // Returns cursor position change over the last frame.
    {
        if (!s_InputInstance) return {0,0};
        return s_InputInstance->m_mouseDelta;
    }

    double InputSystem::GetMouseScrollDelta()   // Returns scroll wheel delta over the last frame.
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

    /*************** CURSOR (cursor management) */

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

    void InputSystem::SetCursorVisible(bool InVisible) // Set cursor visibility.
    {
        if (s_InputInstance && s_InputInstance->GetInputListen())
        {
            s_InputInstance->GetInputListen()->SetCursorVisible(InVisible);
            ENGINE_LOG_INFO("Cursor visible: {}", InVisible);
        }
    }

    void InputSystem::SetCursorMode(int mode)   // Set cursor mode: 0 - normal, 1 - hidden, 2 - disabled.
    {
        if (s_InputInstance && s_InputInstance->GetInputListen())
        {
            s_InputInstance->GetInputListen()->SetCursorMode(mode);
            ENGINE_LOG_INFO("Cursor mode: {}", mode);
        }
    }

    /*************** METODS (other methods) */

    InputSystem& InputSystem::GetInstance() // Returns reference to the single instance.
    {
        return *s_InputInstance;
    }

    void InputSystem::SetKeyState(InputKey Key, InputState State)   // Sets the state of a specific key (internal method).
    {
        auto idx = static_cast<uint32_t>(Key);
        if (idx < static_cast<uint32_t>(InputKey::KeyCount))
        {
            m_keyStates[idx] = static_cast<uint8_t>(State);
        }
    }

    InputState InputSystem::GetKeyStateInternal(InputKey Key) const // Internal method for getting key state.
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

    /*switch (State)
    {
            case InputState::Pressed:
                s_InputInstance->OnKeyPressed().Broadcast(key, scancode, mods);
                break;
            case InputState::Released:
                s_InputInstance->OnKeyReleased().Broadcast(key, scancode);
                break;
            case InputState::Repeated:
                //s_InputInstance->OnKeyPressed().Broadcast(key, scancode, mods);
                break;
            case InputState::Unknown:
                break;
            
            default:
                break;
    }*/

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
    //ENGINE_LOG_TRACE("Mouse Move: x {} y {}",LNewX,LNewY);
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

    switch (State)
    {
            case InputState::Pressed:
                s_InputInstance->OnMouseButtonPressed().Broadcast(button, mods);
                break;
            case InputState::Released:
                s_InputInstance->OnMouseButtonReleased().Broadcast(button, mods);
                break;
            default:
                break;
    }

    //s_InputInstance->OnMouseButtonPressed().Broadcast(button, mods);
}
