// (c) Nikita Rogalev. All rights reserved.

#include "Input.h"                                 // Main input system header
#include "Engine/Core/Base/EngineCore.h"

namespace Engine
{
    //InputSystem* InputSystem::s_InputInstance = nullptr;        // Static pointer initialization for the single InputSystem instance.

    /*************** BASE (basic init/shutdown methods) */

    void InputSystem::Init()    // Initialize input system.
    {
        // Zero out key state arrays (current and previous).
        for (auto& LState : m_keyStates) LState = 0;
        for (auto& LState : m_prevKeyStates) LState = 0;

        if (!Engine::EngineCore::GetEngineContext().GetWindowManager()) return;

        m_DH_OnHasFocusChange = Engine::EngineCore::GetEngineContext().GetWindowManager()->OnHasFocusChange().Subscribe(this, &InputSystem::CallOnHasFocusChange);
        m_DH_OnWinCreate = Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinCreate().Subscribe(this, &InputSystem::CallOnWinCreate);
        m_DH_OnWinDestroy = Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinDestroy().Subscribe(this, &InputSystem::CallOnWinDestroy);

        ENGINE_LOG_TRACE("Input system start!");
    }

    void InputSystem::Shutdown()  // Shutdown input system.
    {
        Engine::EngineCore::GetEngineContext().GetWindowManager()->OnHasFocusChange().Unsubscribe(m_DH_OnHasFocusChange);
        Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinCreate().Unsubscribe(m_DH_OnWinCreate);
        Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinDestroy().Unsubscribe(m_DH_OnWinDestroy);

        m_InputListenList.clear();

        ENGINE_LOG_INFO("Input system shutdown");
    }

    void InputSystem::Update()                          // Update input state (called every frame).
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return;

        // Save current states as "previous" for the next frame.
        memcpy(LInputSystem->m_prevKeyStates, LInputSystem->m_keyStates, sizeof(LInputSystem->m_keyStates));
        // Reset mouse delta and scroll, as they accumulate only for one frame.
        LInputSystem->m_mouseDelta={0,0};
        LInputSystem->m_scrollDelta=0.0;
        ENGINE_LOG_TRACE("Mouse Move: clear");   
    }

    void InputSystem::InitNewWindow(TWeak<Engine::IWindow> InWinHandle, Engine::WindowContext InWinContext)
    {
        TRef<IInputListen> LIL = InputListenAPIFactory::create();
  
        if (!LIL.get())
        {
            ENGINE_LOG_ERROR("ERROR SERACH INPUT LISTEN!");
            return;
        }

        LIL.get()->Init(InWinHandle);
        LIL.get()->OnKeyPressed().Subscribe([&](InputKey key, int mods, int repeat,InputState state) {CallOnKeyPressed(key,mods,repeat,state);});
        LIL.get()->OnMouseButtonPressed().Subscribe([&](InputKey button, int mods,InputState state) {CallOnMouseButtonPressed(button,mods,state);});
        LIL.get()->OnMouseMoved().Subscribe([&](float x, float y) {CallOnMouseMoved(x,y);});
        LIL.get()->OnMouseScrolled().Subscribe([&](float x, float y) {CallOnMouseScrolled(x,y);});
        LIL.get()->OnCharInput().Subscribe([&](uint32_t codepoint) {OnCharInput().Broadcast(codepoint);});

        m_InputListenList.insert({InWinContext, std::move(LIL)});
        ENGINE_LOG_TRACE("Input init for window: {0}", InWinContext.WindowID);
    }

    void InputSystem::DeInitWindow(TWeak<Engine::IWindow> InWinHandle, Engine::WindowContext InWinContext)
    {
        auto it = m_InputListenList.find(InWinContext);
        if (it == m_InputListenList.end()) 
        {
            ENGINE_LOG_WARN("Error DeInit input window [{0}]. There is no such window.",InWinContext.WindowID);
            return;
        }

        it->second.get()->OnKeyPressed().Clear();
        it->second.get()->OnMouseButtonPressed().Clear();
        it->second.get()->OnMouseMoved().Clear();
        it->second.get()->OnMouseScrolled().Clear();
        it->second.get()->OnCharInput().Clear();
        it->second.get()->DeInit();
        m_InputListenList.erase(InWinContext);
    }

    /*************** INPUTS (key state polling) */

    bool InputSystem::IsKeyPressed(InputKey Key)    // Check if a key is held down in the current frame (including repeat).
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return false;

        if (!LInputSystem->GetInputListenFromActivWin()) return false;
        return LInputSystem->GetKeyStateInternal(Key) == InputState::Pressed || LInputSystem->GetKeyStateInternal(Key) == InputState::Repeated;
    }

    bool InputSystem::IsKeyJustPressed(InputKey Key)    // Check if a key was just pressed (this frame).
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return false;

        if (!LInputSystem->GetInputListenFromActivWin()) return false;
        return LInputSystem->GetKeyStateInternal(Key) == InputState::Pressed && LInputSystem->m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }

    bool InputSystem::IsKeyJustReleased(InputKey Key)   // Check if a key was just released.
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return false;

        if (!LInputSystem->GetInputListenFromActivWin()) return false;
        return LInputSystem->GetKeyStateInternal(Key) == InputState::Released || LInputSystem->m_prevKeyStates[static_cast<uint32_t>(Key)] == 0;
    }


    InputState InputSystem::GetKeyState(InputKey Key)   // Get current key state.
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return InputState::Unknown;

        if (!LInputSystem->GetInputListenFromActivWin()) InputState::Unknown;
        return LInputSystem->GetKeyStateInternal(Key);
    }

    /*************** MOUSE (mouse polling) */

    bool InputSystem::IsMouseButtonPressed(InputKey Button) {return IsKeyPressed(Button);};             // Check if a mouse button is held (uses same mechanism as keys).
    bool InputSystem::IsMouseButtonJustPressed(InputKey Button) {return IsKeyJustPressed(Button);};     // Check if a mouse button was just pressed.

    Vector2 InputSystem::GetMousePosition()   // Returns current cursor position.
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return {0,0};

        return LInputSystem->m_mousePosition;
    };

    Vector2 InputSystem::GetMouseDelta()      // Returns cursor position change over the last frame.
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return {0,0};

        return LInputSystem->m_mouseDelta;
    }

    double InputSystem::GetMouseScrollDelta()   // Returns scroll wheel delta over the last frame.
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return 0,0;

        return LInputSystem->m_scrollDelta;
    }

    float InputSystem::GetMouseSensivity()
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return 0,0;

        return LInputSystem->m_mouseSensivity;
    }

    bool InputSystem::IsInverMoveY()
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return false;

        return LInputSystem->m_Invers_Y;
    }

    /*************** CURSOR (cursor management) */

    bool InputSystem::GetCursorVisible()
    {
        InputSystem* LInputSystem = GetInstance();
        if (LInputSystem && LInputSystem->GetInputListenFromActivWin())
        {
            return LInputSystem->GetInputListenFromActivWin()->GetCursorVisible();
        }
        ENGINE_LOG_WARN("No search InputListen!");
        return false;
    }

    int InputSystem::GetCursorMode()
    {
        InputSystem* LInputSystem = GetInstance();
        if (LInputSystem && LInputSystem->GetInputListenFromActivWin())
        {
            return LInputSystem->GetInputListenFromActivWin()->GetCursorMode();
        }
        ENGINE_LOG_WARN("No search InputListen!");
        return 0;
    }

    void InputSystem::SetCursorVisible(bool InVisible) // Set cursor visibility.
    {
        InputSystem* LInputSystem = GetInstance();
        if (LInputSystem && LInputSystem->GetInputListenFromActivWin())
        {
            LInputSystem->GetInputListenFromActivWin()->SetCursorVisible(InVisible);
            ENGINE_LOG_INFO("Cursor visible: {}", InVisible);
        }
    }

    void InputSystem::SetCursorMode(int mode)   // Set cursor mode: 0 - normal, 1 - hidden, 2 - disabled.
    {
        InputSystem* LInputSystem = GetInstance();
        if (LInputSystem && LInputSystem->GetInputListenFromActivWin())
        {
            LInputSystem->GetInputListenFromActivWin()->SetCursorMode(mode);
            ENGINE_LOG_INFO("Cursor mode: {}", mode);
        }
    }

    /*************** METODS (other methods) */

    InputSystem* InputSystem::GetInstance() // Returns reference to the single instance.
    {
        return EngineCore::GetEngineContext().GetInputSystem();
    }

    IInputListen *InputSystem::GetInputListenFromWinContext(Engine::WindowContext InContext)
    {
        auto it = m_InputListenList.find(InContext);
        return (it != m_InputListenList.end()) ? it->second.get() : nullptr;
    }

    IInputListen *InputSystem::GetInputListenFromActivWin()
    {
        if (!Engine::EngineCore::GetEngineContext().GetWindowManager()) return nullptr;
        auto it = m_InputListenList.find(Engine::EngineCore::GetEngineContext().GetWindowManager()->GetFocusWindow());
        return (it != m_InputListenList.end()) ? it->second.get() : nullptr;
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

    
    void InputSystem::CallOnKeyPressed(InputKey key, int scancode, int mods, InputState State)
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return;
        LInputSystem->SetKeyState(key, State);

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

        LInputSystem->OnKeyPressed().Broadcast(key, scancode, mods);
    }

    void InputSystem::CallOnMouseMoved(float x, float y)
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return;
        float LNewX = x-GetMousePosition().x;
        float LNewY = y-GetMousePosition().y;
        LNewY = IsInverMoveY() ? LNewY*-1.f : LNewY;
        //float LDeltaTime = Engine::Time::TimeSystem::GetDeltaTime();
        //float LSensivityMouse = GetMouseSensivity();
        ENGINE_LOG_TRACE("Mouse Move: x {} y {}",LNewX,LNewY);
        LInputSystem->m_mouseDelta = {LNewX, LNewY};
        LInputSystem->OnMouseMoved().Broadcast(LNewX, LNewY);
        LInputSystem->m_mousePosition.x=x;
        LInputSystem->m_mousePosition.y=y;
    }

    void InputSystem::CallOnMouseScrolled(float x, float y)
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return;
        LInputSystem->OnMouseScrolled().Broadcast(x, y);
    }

    void InputSystem::CallOnMouseButtonPressed(InputKey button, int mods, InputState State)
    {
        InputSystem* LInputSystem = GetInstance();
        if (!LInputSystem) return;
        LInputSystem->SetKeyState(button, State);

        switch (State)
        {
                case InputState::Pressed:
                    LInputSystem->OnMouseButtonPressed().Broadcast(button, mods);
                    break;
                case InputState::Released:
                    LInputSystem->OnMouseButtonReleased().Broadcast(button, mods);
                    break;
                default:
                    break;
        }

        //s_InputInstance->OnMouseButtonPressed().Broadcast(button, mods);
    }

    void InputSystem::CallOnHasFocusChange(WindowContext InWinContext)
    {
        
    }

    void InputSystem::CallOnWinCreate(TWeak<Engine::IWindow> InWin, WindowContext InWinContext)
    {
        InitNewWindow(InWin, InWinContext);
    }

    void InputSystem::CallOnWinDestroy(TWeak<Engine::IWindow> InWin, WindowContext InWinContext)
    {
        DeInitWindow(InWin, InWinContext);
    }

}