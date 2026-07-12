// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard
#include "BaseEngine.h"                         // Base engine dependencies
#include "Engine/Input/IInputListen.h"          // Input listener class
#include "Engine/Platform/IWindow.h"            // Window interface
#include "Engine/Platform/WindowManager.h"      // Window manager
#include <glm/glm.hpp>                          // Math library for vectors


struct GLFWwindow;          //TODO: Remove!!!!
//using namespace Input;

namespace Engine
{
    ADD_DELEGATE_THREE_PARAMS(DOnKeyPressed, InputKey, int, int)         // Key press event (key, scancode, mods)
    ADD_DELEGATE_TWO_PARAMS(DOnKeyReleased,InputKey,int)                 // Key release event (key, mods)
    ADD_DELEGATE_TWO_PARAMS(DOnMouseMoved,float,float)              // Mouse move event (x, y)
    ADD_DELEGATE_TWO_PARAMS(DOnMouseScrolled,float,float)           // Scroll wheel event (xOffset, yOffset)
    ADD_DELEGATE_TWO_PARAMS(DOnMouseButtonPressed,InputKey,int)          // Mouse button press event (button, mods)
    ADD_DELEGATE_TWO_PARAMS(DOnMouseButtonReleased,InputKey,int)         // Mouse button release event (button, mods)
    ADD_DELEGATE_ONE_PARAM(DOnCharInput, uint32_t)                      // Char input (unicode codepoint)
    
    class Window;

    /**
     * @class InputSystem
     * @brief Input system (keyboard, mouse). Implemented as a singleton.
     *
     * Provides static methods for querying key and mouse state,
     * as well as delegates for subscribing to input events.
     * Supports two modes: direct polling (IsKeyPressed, etc.)
     * and event-driven (via delegates).
     */
    class InputSystem
    {
    public:
        InputSystem() = default;
        ~InputSystem() = default;

        void Init();                     // Initialize input system.
        void Shutdown();                 // Shutdown input system, free resources.
        static void Update();                                               // Update input state (should be called every frame).

        void InitNewWindow(TWeak<Engine::IWindow> InWinHandle, Engine::WindowContext InWinContext);
        void DeInitWindow(TWeak<Engine::IWindow> InWinHandle, Engine::WindowContext InWinContext);

        // Keyboard
        static bool IsKeyPressed(InputKey Key);                             // Checks if a key is held in the current frame.
        static bool IsKeyJustPressed(InputKey Key);                         // Checks if a key was pressed this frame (just now).
        static bool IsKeyJustReleased(InputKey Key);                        // Checks if a key was released this frame.
        static InputState GetKeyState(InputKey Key);                        // Returns current key state (InputState).

        // Mouse
        static bool IsMouseButtonPressed(InputKey Button);                  // Checks if a mouse button is held.
        static bool IsMouseButtonJustPressed(InputKey Button);              // Checks if a mouse button was pressed this frame.
        static Vector2 GetMousePosition();                                // Returns current cursor position in window coordinates.
        static Vector2 GetMouseDelta();                                   // Returns mouse position change over the last frame (delta).
        static double GetMouseScrollDelta();                                // Returns scroll wheel delta.
        static float GetMouseSensivity();
        static bool IsInverMoveY();

        // Cursor
        static bool GetCursorVisible();                         
        static int GetCursorMode();                         
        static void SetCursorVisible(bool Visible);                         // Sets cursor visibility.
        static void SetCursorMode(int Mode);                                // Sets cursor mode: 0=Normal, 1=Hidden, 2=Disabled

        // Singleton
        static InputSystem* GetInstance();                                  // Returns reference to the single InputSystem instance.

        static void* GetWindowHandle();                                     // Returns pointer to native window handle (e.g., GLFWwindow*).

        IInputListen* GetInputListenFromWinContext(Engine::WindowContext InContext); 
        IInputListen* GetInputListenFromActivWin();

        // Delegate access for subscription
        DOnKeyPressed& OnKeyPressed() {return s_OnKeyPressed;}
        DOnKeyReleased& OnKeyReleased() {return s_OnKeyReleased;}
        DOnMouseMoved& OnMouseMoved() {return s_OnMouseMoved;}
        DOnMouseScrolled& OnMouseScrolled() {return s_OnMouseScrolled;}
        DOnMouseButtonPressed& OnMouseButtonPressed() {return s_OnMouseButtonPressed;}
        DOnMouseButtonReleased& OnMouseButtonReleased() {return s_OnMouseButtonReleased;}
        DOnCharInput& OnCharInput() {return s_OnCharInput;}

    protected:
        //TUniquePtr<IInputListen> m_InputListen=nullptr;                 // Input listener
        unordered_map<Engine::WindowContext, TRef<IInputListen>,Engine::WindowContextHash> m_InputListenList;
    private:

        void SetKeyState(InputKey Key, InputState State);                   // Sets the state of the specified key.
        InputState GetKeyStateInternal(InputKey Key) const;                 // Internal method for getting key state.

        //static InputSystem* s_InputInstance;                                // Pointer to the single instance (singleton).

        // Key states: current and previous (for determining just pressed/released).
        uint8_t m_keyStates[static_cast<uint32_t>(InputKey::KeyCount)];
        uint8_t m_prevKeyStates[static_cast<uint32_t>(InputKey::KeyCount)];

        // Mouse state
        Vector2 m_mousePosition = {0,0};                                  // Current cursor position.
        Vector2 m_mouseDelta = {0,0};                                     // Position change per frame.
        double m_scrollDelta = 0.0;                                         // Accumulated scroll wheel delta.
        float m_mouseSensivity = 0.2f;                                      // Mouse sensitivity
        bool m_Invers_Y = true;

        // Friend class Window has access to private methods (e.g., for processing).
        friend class Window;

        // Delegate instances (events).
        DOnKeyPressed s_OnKeyPressed;
        DOnKeyReleased s_OnKeyReleased;                     // keyCode, mods
        DOnMouseMoved s_OnMouseMoved;                       // x, y
        DOnMouseScrolled s_OnMouseScrolled;                 // xOffset, yOffset
        DOnMouseButtonPressed s_OnMouseButtonPressed;       // button, mods
        DOnMouseButtonReleased s_OnMouseButtonReleased;     // button, mods
        DOnCharInput s_OnCharInput;                         // char input

        //Delegate
        Engine::DelegateHandle m_DH_OnHasFocusChange;
        Engine::DelegateHandle m_DH_OnWinCreate;
        Engine::DelegateHandle m_DH_OnWinDestroy;

    protected:
        void CallOnKeyPressed(InputKey key, int scancode, int mods, InputState State);
        void CallOnMouseMoved(float x, float y);
        void CallOnMouseScrolled(float x, float mods);
        void CallOnMouseButtonPressed(InputKey button, int mods, InputState State);
    public:
        void CallOnHasFocusChange(WindowContext InWinContext);
        void CallOnWinCreate(TWeak<Engine::IWindow> InWin, WindowContext InWinContext);
        void CallOnWinDestroy(TWeak<Engine::IWindow> InWin, WindowContext InWinContext);
    };
}
