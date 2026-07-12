// (c) Nikita Rogalev. All rights reserved.

#include "IInputListen.h"                                         // Header with IInputListen interface definition and InputListenAPIFactory
#include "Platform/WindowAPI/WindowGLFW/InputGLFW.h"              // Concrete InputListen implementation for GLFW

namespace Engine 
{
    /**
     * @brief Creates an InputListen class instance depending on platform/API.
     *
     * Factory method implementation. Depending on platform macros
     * (e.g., ENGINE_WINDOW_GLFW), returns the appropriate object implementing
     * the InputListen interface.
     *
     * @return TUniquePtr<InputListen> Unique pointer to the created object.
     *         Returns nullptr if no platform is defined.
     */
    TRef<IInputListen> InputListenAPIFactory::create()
    {
        #ifdef ENGINE_WINDOW_GLFW
        // If using GLFW, create GLFW-specific input listener.
        return CreateRef<InputListenGLFWSystem>();
        #endif
        // Return null pointer if no supported platform is found.
        // Additional branches can be added here in the future (e.g., Windows Raw Input).
        return nullptr;
    }
}
