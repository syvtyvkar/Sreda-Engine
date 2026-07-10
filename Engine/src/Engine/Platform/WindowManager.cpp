// (c) Nikita Rogalev. All rights reserved.

#include "WindowManager.h"                                   // Concrete window implementation for GLFW
#include "IWindow.h"                                         // Include window interface (abstract Window class)
#include "Engine/Core/Base/EngineCore.h"
#include "Engine/UI/Framework/UISystem.h"
#include "Engine/UI/Framework/IUIContext.h"

namespace Engine 
{
    WindowManager::~WindowManager()
    {
        std::vector<WindowContext> contexts;
        contexts.reserve(m_windowList.size());
        for (const auto& pair : m_windowList)
            contexts.push_back(pair.first);

        for (const auto& ctx : contexts)
            CloseEngineWindow(ctx);
    }

    IWindow* WindowManager::GetEngineWindow(WindowContext InContext)
    {
        auto it = m_windowList.find(InContext);
        return (it != m_windowList.end()) ? it->second.get() : nullptr;
    }

    WindowContext WindowManager::CreateEngineWindow(const WindowConfig InConfigWindow, bool InMainWindow)
    {
        WindowConfig LResultContext = InConfigWindow;
        LResultContext.cntx = WindowContext(CountWindowID);
        CountWindowID++;

        TUniquePtr<IWindow> LW = WindowAPIFactory::create();
        if (!LW) 
        {
            ENGINE_LOG_CRITICAL("Failed to create window!");
            ENGINE_ASSERT(false, "Failed to create window!");
        }
        LW->Init(LResultContext);

        LW.get()->OnHasFocusChange().Subscribe(this, &WindowManager::CallWindowFocusChange);
        LW.get()->OnMinimizedChange().Subscribe(this, &WindowManager::CallWindowMinimized);
        LW.get()->OnWindowReSize().Subscribe(this, &WindowManager::CallWindowReSize);
        LW.get()->LoadIconFromFile();

        m_windowList.insert({LResultContext.cntx, std::move(LW)});
        OnWinCreate().Broadcast(GetEngineWindow(LResultContext.cntx),LResultContext.cntx);

        if (InConfigWindow.widget)
        {
            EngineCore::GetEngineContext().GetUISystem()->RegisterWidget(LResultContext.cntx, InConfigWindow.widget);
            EngineCore::GetEngineContext().GetUISystem()->GetContextFromWindowContext(LResultContext.cntx)->GetRootWidget()->OnInit();
        }

        m_focusWindow=LResultContext.cntx;

        if (InMainWindow) m_mainWindow = LResultContext.cntx;

        return LResultContext.cntx;
    }

    void WindowManager::CloseEngineWindow(WindowContext InContext)
    {
        auto& it = m_windowList.find(InContext);
        if (it == m_windowList.end()) return;

        OnWinDestroy().Broadcast(it->second.get(), InContext);
        it->second->Close();
        if (m_mainWindow == InContext)
        {
            if (m_windowList.size() <= 1)
            {
                m_windowList.erase(it);
                return;
            }

            for (auto& [LKey,LVal] : m_windowList)
            {
                if (!LVal) continue;
                CloseEngineWindow(LKey);
            }
            it->second->WindowTerminate();
            m_windowList.clear();
        }
        else
        {
            m_windowList.erase(it);
        }
    }

    bool WindowManager::ShouldClose() const
    {
        if (m_windowList.empty()) return true;

        for (auto& [LKey,LVal] : m_windowList)
        {
            if (!LVal) continue;

            if (LVal.get()->ShouldClose()) return true;
        }

        return false;
    }

    void WindowManager::BeginRender()
    {
        if (!GetEngineWindow(m_focusWindow)) return;
        GetEngineWindow(m_focusWindow)->BeginRender();
    }

    void WindowManager::EndRender()
    {
        if (!GetEngineWindow(m_focusWindow)) return;
        GetEngineWindow(m_focusWindow)->EndRender();
    }

    void WindowManager::Update()
    {
        if (!GetEngineWindow(m_focusWindow)) return;
        GetEngineWindow(m_focusWindow)->Update();
    }

    int WindowManager::GetWidth(const WindowContext& InContext)
    {
        if (!GetEngineWindow(InContext)) return 0;
        return GetEngineWindow(InContext)->GetWidth();
    }

    int WindowManager::GetHeight(const WindowContext& InContext)
    {
        if (!GetEngineWindow(InContext)) return 0;
        return GetEngineWindow(InContext)->GetHeight();
    }

    WindowMode WindowManager::GetWindowMode(const WindowContext& InContext)
    {
        if (!GetEngineWindow(InContext)) return WindowMode::Window;
        return GetEngineWindow(InContext)->GetWindowMode();
    }

    void WindowManager::SetWindowMode(const WindowContext& InContext, WindowMode NewMode)
    {
        if (!GetEngineWindow(InContext)) return;
        GetEngineWindow(InContext)->SetWindowMode(NewMode);
    }

    void WindowManager::CallWindowReSize(WindowContext InContext, int InX,int InY)
    {
        OnWindowReSize().Broadcast(InContext, InX,InY);
        ENGINE_LOG_TRACE("Window {0} resized to {1}x{2}", InContext.WindowID, InX, InY);
    }

    void WindowManager::CallWindowFocusChange(WindowContext InContext,bool InVal)
    {
        OnHasFocusChange().Broadcast(InContext);
        if (InVal)
        {
            m_focusWindow = InContext;
            ENGINE_LOG_TRACE("Window {0} focus changed", InContext.WindowID);
        }
    }

    void WindowManager::CallWindowMinimized(WindowContext InContext,bool InVal)
    {
        OnMinimizedChange().Broadcast(InContext);
        ENGINE_LOG_TRACE("Window {0} minimized changed", InContext.WindowID);
    }
}