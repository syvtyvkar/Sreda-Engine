// (c) Nikita Rogalev. All rights reserved.

#include "UISystem.h"
#include "UIBuilder.h"

#include "Engine/Core/Base/EngineCore.h"

namespace Engine::UI 
{
    void UISystem::ToggleDebugger()
    {

    }

    bool UISystem::AddButtonContext(std::string InNameButton, DOnUIWidgetClick& Handle)
    {
        if (m_contextMenu)
        {
            //Handle = m_contextMenu.get()->AddButtonContext(InNameButton);
            return true;
        }
        return false;
    }

    void UISystem::RemoveButtonContext(std::string InNameButton)
    {
        if (m_contextMenu)
        {
            return m_contextMenu.get()->RemoveButtonContext(InNameButton);
        }
    }

    void UISystem::ClearAllButton()
    {
        if (m_contextMenu)
        {
            return m_contextMenu.get()->ClearAllButton();
        }
    }

    void UISystem::CallOnWinCreate(IWindow *InWin, WindowContext InWinContext)
    {
        CreateContextFromWindowContext(InWinContext);
    }

    void UISystem::CallOnWinDestroy(IWindow *InWin, WindowContext InWinContext)
    {
        DestroyContextFromWindowContext(InWinContext);
    }

    bool UISystem::Initialize()
    {
        /*if (!CreateContext())
        {
            return false;
        }*/
        m_contextMenu = CreateUniquePtr<UIContextMenu>();

        m_DH_OnWinCreate = Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinCreate().Subscribe(this, &UISystem::CallOnWinCreate);
        m_DH_OnWinDestroy = Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinDestroy().Subscribe(this, &UISystem::CallOnWinDestroy);

        return true;
    }

    void UISystem::Shutdown()
    {
        if (m_contextMenu)
        {
            m_contextMenu.get()->ClearAllButton();
            m_contextMenu.reset();
        }
        Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinCreate().Unsubscribe(m_DH_OnWinCreate);
        Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinDestroy().Unsubscribe(m_DH_OnWinDestroy);
        m_ui_contexts.clear();
        //DestroyContext();
    }
    
    void UISystem::Update(float DeltaTime)
    {
        for (auto& [LKey, LVal] : m_ui_contexts)
        {
            if (!LVal) continue;
            LVal.get()->Update(DeltaTime);
        }
        if (m_contextMenu.get())
        {
            m_contextMenu.get()->OnUpdate(DeltaTime);
        }
    }

    void UISystem::Render()
    {
        for (auto& [LKey, LVal] : m_ui_contexts)
        {
            if (!LVal) continue;
            LVal.get()->Render();
        }
        if (m_contextMenu.get())
        {
            m_contextMenu.get()->OnRender();
        }
    }

    void UISystem::BeginFrame()
    {
        for (auto& [LKey, LVal] : m_ui_contexts)
        {
            if (!LVal) continue;
            LVal.get()->BeginFrame();
        }
    }

    void UISystem::EndFrame()
    {
        for (auto& [LKey, LVal] : m_ui_contexts)
        {
            if (!LVal) continue;
            LVal.get()->EndFrame();
        }
    }

    void UISystem::CreateContextFromWindowContext(WindowContext InContext)
    {
        TUniquePtr<UIContext> LUIContext = UIContextFactory::create();
        ENGINE_ASSERT(LUIContext, "No UI context! The UI System cannot be initialized!");
        LUIContext.get()->InitContext(InContext);

        m_ui_contexts.insert({InContext, std::move(LUIContext)});
    }

    UIContext *UISystem::GetContextFromWindowContext(WindowContext InContext)
    {
        auto it = m_ui_contexts.find(InContext);
        return (it != m_ui_contexts.end()) ? it->second.get() : nullptr;
    }

    void UISystem::DestroyContextFromWindowContext(WindowContext InContext)
    {
        auto it = m_ui_contexts.find(InContext);
        if (it == m_ui_contexts.end()) return;

        m_ui_contexts.erase(InContext);
    }

    void UISystem::RegisterWidget(WindowContext InWinContext,TRef<UIElement> widget)
    {
        if (widget.get() == nullptr) return;

        UIContext* LCntxt = GetContextFromWindowContext(InWinContext);

        if (LCntxt)
        {
            LCntxt->RegisterWidget(widget);
        }
    }

    void UISystem::RemoveWidget(WindowContext InWinContext,const TRef<UIElement> &widget)
    {
        if (widget.get() == nullptr) return;

        UIContext* LCntxt = GetContextFromWindowContext(InWinContext);
        
        if (LCntxt)
        {
            LCntxt->RemoveWidget(widget);
        }
    }
    UIWidget *UISystem::GetFocusWidget(WindowContext InWinContext)
    {
        UIContext* LCntxt = GetContextFromWindowContext(InWinContext);
        if (LCntxt)
        {
            return LCntxt->GetFocusWidget();
        }
        return nullptr;
    }

    void UISystem::SetFocusWidget(WindowContext InWinContext,TRef<UIWidget> InNewFocus)
    {
        UIContext* LCntxt = GetContextFromWindowContext(InWinContext);
        if (LCntxt)
        {
            return LCntxt->SetFocusWidget(InNewFocus);
        }
    }

    UIWidget *UISystem::GetHoverWidget(WindowContext InWinContext)
    {
        UIContext* LCntxt = GetContextFromWindowContext(InWinContext);
        if (LCntxt)
        {
            return LCntxt->GetHoverWidget();
        }
        return nullptr;
    }

    void UISystem::SetHoverWidget(WindowContext InWinContext,TRef<UIWidget> InNewHover)
    {
        UIContext* LCntxt = GetContextFromWindowContext(InWinContext);
        if (LCntxt)
        {
            LCntxt->SetHoverWidget(InNewHover);
        }
    }
}