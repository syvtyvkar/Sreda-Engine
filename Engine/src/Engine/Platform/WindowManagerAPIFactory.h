// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Multiple inclusion guard for header file

#include "BaseEngine.h"                 // Base engine dependencies
#include <map>
#include <memory>                                   
#include "IWindow.h"
#include "WindowManager.h"

namespace Engine
{

    /**
     * @class WindowManagerAPIFactory
     * @brief Factory for creating a window instance depending on platform/API.
     *
     * Hides the details of the specific window implementation behind a unified Window interface.
     */
    class WindowManagerAPIFactory 
    {
    public:
        /** 
         * @brief Creates a unique pointer to a window object.
         * @return TUniquePtr<Window> Pointer to the created window.
         *
         * Depending on macros (e.g., ENGINE_WINDOW_GLFW) returns the corresponding
         * Window descendant (e.g., WindowGLF3). If no platform is defined,
         * may return nullptr.
         */
        static TUniquePtr<WindowManager> createWindowManager();

        /**
         * @brief Creates a unique pointer to a window object.
         * @return TUniquePtr<Window> Pointer to the created window.
         *
         * Depending on macros (e.g., ENGINE_WINDOW_GLFW) returns the corresponding
         * Window descendant (e.g., WindowGLF3). If no platform is defined,
         * may return nullptr.
         */
        static TRef<IWindow> createWindow();
    };
}