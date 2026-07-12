// (c) Nikita Rogalev. All rights reserved.

#include "WindowManager.h"                                   // Concrete window implementation for GLFW
#include "IWindow.h"                                         // Include window interface (abstract Window class)
#include "Engine/Core/Base/EngineCore.h"
#include "Engine/UI/Framework/UISystem.h"
#include "Engine/UI/Framework/IUIContext.h"
#include "WindowManagerAPIFactory.h"

namespace Engine 
{
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
}