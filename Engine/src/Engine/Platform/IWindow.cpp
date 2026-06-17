// (c) Nikita Rogalev. All rights reserved.

// Window Class
#include "IWindow.h"                                         // Include window interface (abstract Window class)
#include "Platform/WindowAPI/WindowGLFW/WindowGLFW.h"        // Concrete window implementation for GLFW

namespace Engine 
{

    /**
     * @brief Factory method implementation for creating a window.
     *
     * Depending on the defined platform macro (here ENGINE_WINDOW_GLFW),
     * creates and returns the appropriate window object.
     * Returns nullptr if the macro is not defined.
     *
     * @return TUniquePtr<Window> Unique pointer to the created window,
     *         or nullptr if the platform is not supported.
     */
    TUniquePtr<Window> WindowAPIFactory::create() 
    {
        #ifdef ENGINE_WINDOW_GLFW
        return CreateUniquePtr<WindowGLFW>();  // Create window for GLFW
        #endif
        return nullptr;
    }

}
