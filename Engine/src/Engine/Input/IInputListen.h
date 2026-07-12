// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Multiple inclusion guard

#include "BaseEngine.h"                 // Base engine dependencies
#include <memory>                       
#include <string>                       // For std::string

namespace Engine 
{
    // Delegate type definitions for input events specific to InputListen.
    ADD_DELEGATE_FOUR_PARAMS(DOnKeyPressedListen, InputKey, int, int,InputState)       // key, scancode, mods
    ADD_DELEGATE_TWO_PARAMS(DOnMouseMovedListen,float,float)            // x, y
    ADD_DELEGATE_TWO_PARAMS(DOnMouseScrolledListen,float,float)         // xOffset, yOffset
    ADD_DELEGATE_THREE_PARAMS(DOnMouseButtonPressedListen,InputKey,int,InputState)        // button, mods
    ADD_DELEGATE_ONE_PARAM(DOnCharInputListen, uint32_t)               // char input (unicode codepoint)

    /**
     * @class InputListen
     * @brief Abstract base class for input handlers.
     *
     * Provides an interface for creating objects that can subscribe
     * to input events and process them. Used to decouple input processing
     * logic from the specific platform or API.
     *
     * Contains virtual Init/DeInit methods and a set of delegates
     * through which input event notifications can be received.
     */
    class IInputListen
    {
    public:

        //virtual IInputListen() = default;
        virtual ~IInputListen() = default;                                                               // Virtual destructor for correct deletion of derived classes

        virtual void Init(TWeak<class IWindow> InWindow) = 0;                                              // Initialize listener.
        virtual void DeInit() = 0;                                                                      // Deinitialize listener, free resources.

        // Delegate access methods for subscription.
        DOnKeyPressedListen& OnKeyPressed() {return s_OnKeyPressedListen;}
        DOnMouseMovedListen& OnMouseMoved() {return s_OnMouseMovedListen;}
        DOnMouseScrolledListen& OnMouseScrolled() {return s_OnMouseScrolledListen;}
        DOnMouseButtonPressedListen& OnMouseButtonPressed() {return s_OnMouseButtonPressedListen;}
        DOnCharInputListen& OnCharInput() {return s_OnCharInputListen;}

        virtual bool GetCursorVisible() = 0;                    
        virtual int GetCursorMode() = 0;
        virtual void SetCursorVisible(bool Visible) = 0;                    
        virtual void SetCursorMode(int Mode) = 0; // 0=Normal, 1=Hidden, 2=Disable

    protected:

        // Protected delegate instances, accessible to derived classes.
        DOnKeyPressedListen s_OnKeyPressedListen;
        DOnMouseMovedListen s_OnMouseMovedListen;
        DOnMouseScrolledListen s_OnMouseScrolledListen;
        DOnMouseButtonPressedListen s_OnMouseButtonPressedListen;
        DOnCharInputListen s_OnCharInputListen;
    };

    /**
     * @class InputListenAPIFactory
     * @brief Factory for creating InputListen instances depending on platform.
     *
     * Encapsulates the logic for selecting a specific InputListen implementation
     * (e.g., InputGLF3 for GLFW) and hides it behind a unified interface.
     */
    class InputListenAPIFactory 
    {
    public:
        /**
         * @brief Creates a unique pointer to an InputListen object.
         * @return TUniquePtr<InputListen> Pointer to the created instance.
         */
        static TRef<IInputListen> create();
    };
}
