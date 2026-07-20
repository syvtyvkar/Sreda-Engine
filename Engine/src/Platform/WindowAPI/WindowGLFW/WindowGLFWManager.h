// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Multiple inclusion guard for header file

#ifdef ENGINE_WINDOW_GLFW
#include <GLFW/glfw3.h>
#endif

#include "BaseEngine.h"                 // Base engine dependencies
#include <map>
#include <memory>                                   
#include "Engine/Platform/WindowManager.h"

namespace Engine
{
    class WindowGLFWManager : public WindowManager
    {
        std::unordered_map<WindowContext, TRef<IWindow>, WindowContextHash> m_windowList = {};
        WindowContext m_focusWindow = WindowContext(-1);
        WindowContext m_mainWindow = WindowContext(-1);
    public:
        WindowGLFWManager();
        virtual ~WindowGLFWManager() override;

        virtual WindowContext GetFocusWindow() const override {return m_focusWindow;};
        virtual void SetFocusWindow(WindowContext InFocus) override {m_focusWindow = InFocus;}
        virtual WindowContext GetMainWindow() const override {return m_mainWindow;};
        virtual void SetMainWindow(WindowContext InFocus) override {m_mainWindow = InFocus;}

        virtual Engine::IWindow* GetEngineWindow(WindowContext InContext = WindowContext(0)) override;
        virtual WindowContext CreateEngineWindow(const struct WindowConfig InConfigWindow, bool InMainWindow = false) override;
        virtual void CloseEngineWindow(WindowContext InContext = WindowContext(0)) override;

        virtual bool ShouldClose() const override;

        virtual void PollEvents() override;

        /**
         * @brief Global frame update all windows
         */
        virtual void FrameUpdate(float InDeltaTime) override;

        static void FocusCallback(GLFWwindow* window, int focused);
        static void IconifyCallback(GLFWwindow* window, int iconified);
        static void CloseWindowCallback(GLFWwindow* window);
        static void FramebufferResizeCallback(GLFWwindow* Window, int width, int height);
    };
}