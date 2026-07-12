// (c) Nikita Rogalev. All rights reserved.

#include "WindowManagerAPIFactory.h"                              
#include "Platform/WindowAPI/WindowGLFW/WindowGLFW.h"
#include "Platform/WindowAPI/WindowGLFW/WindowGLFWManager.h"

namespace Engine 
{
    TUniquePtr<WindowManager> WindowManagerAPIFactory::createWindowManager()
    {
        #ifdef ENGINE_WINDOW_GLFW
        return CreateUniquePtr<WindowGLFWManager>();
        #endif
        return nullptr;
    }
    TRef<IWindow> WindowManagerAPIFactory::createWindow()
    {
        #ifdef ENGINE_WINDOW_GLFW
        return CreateRef<WindowGLFW>();  // Create window for GLFW
        #endif
        return nullptr;
    }
}