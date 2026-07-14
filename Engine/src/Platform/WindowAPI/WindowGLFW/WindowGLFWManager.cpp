// (c) Nikita Rogalev. All rights reserved.

#include <glad/glad.h>                              // TODO: (УБРАТЬ ОТСЮДА!!!!!!!!!!) Загрузчик OpenGL функций

#include "WindowGLFWManager.h"                 // Base engine dependencies

// (c) Nikita Rogalev. All rights reserved.

// Window Class
#include "WindowGLFW.h"                         // Заголовок класса

//#ifdef ENGINE_WINDOW_GLFW
#include <GLFW/glfw3.h>
//#endif

#include "Engine/Core/Base/EngineCore.h"
#include "Engine/Platform/WindowManagerAPIFactory.h"
#include "Engine/Platform/IWindow.h"
#include "Engine/Render/RenderAPIFactory.h"
#include "Engine/Render/RenderCommand.h"

namespace Engine
{
    WindowGLFWManager::WindowGLFWManager()
    {

    }

    WindowGLFWManager::~WindowGLFWManager()
    {
        std::vector<WindowContext> contexts;
        contexts.reserve(m_windowList.size());
        for (const auto& pair : m_windowList)
            contexts.push_back(pair.first);

        for (const auto& ctx : contexts)
            CloseEngineWindow(ctx);
    }

    Engine::IWindow* WindowGLFWManager::GetEngineWindow(WindowContext InContext)
    {
        auto it = m_windowList.find(InContext);
        return (it != m_windowList.end()) ? it->second.get() : nullptr;
    }

    WindowContext WindowGLFWManager::CreateEngineWindow(const WindowConfig InConfigWindow, bool InMainWindow)
    {
        WindowConfig LResultContext = InConfigWindow;
        LResultContext.cntx = WindowContext(CountWindowID);
        CountWindowID++;

        TRef<IWindow> LW = WindowManagerAPIFactory::createWindow();
        if (!LW) 
        {
            ENGINE_LOG_CRITICAL("Failed to create window!");
            ENGINE_ASSERT(false, "Failed to create window!");
        }
        LW->Init(LResultContext);

        GLFWwindow* LWH = static_cast<GLFWwindow*>(LW.get()->GetWindowAPIHandle());

        glfwSetFramebufferSizeCallback(LWH, FramebufferResizeCallback);
        glfwSetWindowFocusCallback(LWH, FocusCallback);
        glfwSetWindowIconifyCallback(LWH, IconifyCallback);
        glfwSetWindowCloseCallback(LWH, CloseWindowCallback);

        LW.get()->LoadIconFromFile();

        TWeak<IWindow> weakLW = LW;
        m_windowList.insert({LResultContext.cntx, std::move(LW)});
        OnWinCreate().Broadcast(weakLW,LResultContext.cntx);

        if (InConfigWindow.widget)
        {
            EngineCore::GetEngineContext().GetUISystem()->RegisterWidget(LResultContext.cntx, InConfigWindow.widget);
            EngineCore::GetEngineContext().GetUISystem()->GetContextFromWindowContext(LResultContext.cntx)->GetRootWidget()->OnInit();
        }

        SetFocusWindow(LResultContext.cntx);

        if (InMainWindow) SetMainWindow(LResultContext.cntx);

        return LResultContext.cntx;
    }

    void WindowGLFWManager::CloseEngineWindow(WindowContext InContext)
    {
        auto& it = m_windowList.find(InContext);
        if (it == m_windowList.end()) return;
    
        TWeak<IWindow> weakLW = it->second;
        GLFWwindow* LWH = static_cast<GLFWwindow*>(it->second.get()->GetWindowAPIHandle());

        glfwSetFramebufferSizeCallback(LWH, nullptr);
        glfwSetWindowFocusCallback(LWH, nullptr);
        glfwSetWindowIconifyCallback(LWH, nullptr);
        glfwSetWindowCloseCallback(LWH, nullptr);

        OnWinDestroy().Broadcast(weakLW, InContext);
        it->second->Close();
        if (GetMainWindow() == InContext)
        {
            if (m_windowList.size() <= 1)
            {
                m_windowList.erase(it);
                return;
            }

            std::vector<WindowContext> contexts;
            contexts.reserve(m_windowList.size());
            for (const auto& pair : m_windowList) 
            {
                if (pair.first != InContext) contexts.push_back(pair.first);
            }

            for (const auto& ctx : contexts) 
            {
                CloseEngineWindow(ctx);  // рекурсивно закрывает все остальные
            }
            auto it_с = m_windowList.find(InContext);
            if (it_с != m_windowList.end()) 
            {
                it_с->second->WindowTerminate();
                m_windowList.erase(it_с);
            }
        }
        else
        {
            m_windowList.erase(it);
        }
    }

    bool WindowGLFWManager::ShouldClose() const
    {
        if (m_windowList.empty()) return true;

        for (auto& [LKey,LVal] : m_windowList)
        {
            if (!LVal) continue;
            if (LKey != GetMainWindow()) continue;

            return LVal.get()->ShouldClose();
        }

        return false;
    }

    void WindowGLFWManager::FrameUpdate(float InDeltaTime)
    {
        if (m_windowList.empty()) return;

        std::vector<TRef<IWindow>> LWindows;

        for (auto& [LKey,LVal] : m_windowList)
        {
            if (!LVal) continue;

            LWindows.push_back(LVal);
        }

        for (TRef<IWindow> LVL : LWindows)
        {
            if (!LVL) continue;

            LVL.get()->FocusThisWindow();
            LVL.get()->Update();
            LVL.get()->BeginRender();
            if (LVL.get()->GetWindowContext() == GetMainWindow())
            {
                EngineCore::GetEngineDelegates().OnAppUpdFrameDelta().Broadcast(Time::TimeSystem::GetDeltaTime()); 
                EngineCore::GetEngineDelegates().OnAppBeginRender().Broadcast();
                EngineCore::GetEngineDelegates().OnAppRender().Broadcast();   
                EngineCore::GetEngineDelegates().OnAppRenderUI().Broadcast();  
                EngineCore::GetEngineDelegates().OnAppEndRender().Broadcast();
            }
            LVL.get()->EndRender();
        }
        glfwPollEvents();
    }

    /**
     * @brief Статический колбэк, вызываемый GLFW при изменении размера фреймбуфера.
     * @param window Указатель на окно GLFW.
     * @param width Новая ширина.
     * @param height Новая высота.
     *
     * Обновляет внутренние размеры, устанавливает viewport OpenGL и вызывает
     * пользовательский колбэк, если он задан.
     */
    void WindowGLFWManager::FramebufferResizeCallback(GLFWwindow* window, int width, int height) 
    {
        auto* win = (class WindowGLFW *)glfwGetWindowUserPointer(window);
        if (win != nullptr) 
        {
            int LW = width;
            int LH = height;
            ENGINE_LOG_TRACE("Window {0} resized to {1}x{2}", win->GetWindowContext().WindowID, LW, LH);
            if (win->GetWindowMode() == WindowMode::Window) 
            {
                win->SetSizeWindow(Vector2(width,height));
            }
            
            // Устанавливаем область вывода на весь новый размер окна
            if (Engine::Render::RenderAPIFactory::GetRenderAPI() != Engine::Render::RenderAPIFactory::RHI_API::Vulkan)  
                RenderCommand::SetViewport(0, 0, width, height);

            EngineCore::GetEngineContext().GetWindowManager()->OnWindowReSize().Broadcast(win->GetWindowContext(), width, height);
            win->OnWindowReSize().Broadcast(width, height);
            if (win->GetWindowMode() == WindowMode::Window) 
            {
                Vector2 LRes = win->GetLocationWindow();
                int LX = (int)LRes.x;
                int LY = (int)LRes.y;
                glfwGetWindowPos(window, &LX, &LY);
            }
        }
    }

    void WindowGLFWManager::FocusCallback(GLFWwindow *window, int focused)
    {
        auto* win = (class WindowGLFW *)glfwGetWindowUserPointer(window);
        if (win != nullptr) 
        {
            win->SetWindowHasFocus((focused == GLFW_TRUE));
            EngineCore::GetEngineContext().GetWindowManager()->OnHasFocusChange().Broadcast(win->GetWindowContext());
            if (focused)
            {
                ENGINE_LOG_TRACE("Window {0} focus changed", win->GetWindowContext().WindowID);
                EngineCore::GetEngineContext().GetWindowManager()->SetFocusWindow(win->GetWindowContext());
            }
        }
    }

    void WindowGLFWManager::IconifyCallback(GLFWwindow *window, int iconified)
    {
        auto* win = (class WindowGLFW *)glfwGetWindowUserPointer(window);
        if (win != nullptr)  
        {
            win->SetWindowMinimized((iconified == GLFW_TRUE));
            ENGINE_LOG_TRACE("Window {0} minimized changed", win->GetWindowContext().WindowID);
            EngineCore::GetEngineContext().GetWindowManager()->OnMinimizedChange().Broadcast(win->GetWindowContext().WindowID);
        }
    }

    void WindowGLFWManager::CloseWindowCallback(GLFWwindow *window)
    {
        auto* win = (class WindowGLFW *)glfwGetWindowUserPointer(window);
        if (win != nullptr)  
        {
            ENGINE_LOG_TRACE("Call window {0} exit", win->GetWindowContext().WindowID);
            EngineCore::GetEngineContext().GetWindowManager()->CloseEngineWindow(win->GetWindowContext());
        }
    }
}