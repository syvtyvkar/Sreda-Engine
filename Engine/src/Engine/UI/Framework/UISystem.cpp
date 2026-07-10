// (c) Nikita Rogalev. All rights reserved.

#include "UISystem.h"
#include "UIBuilder.h"

#include "Engine/Core/Base/EngineCore.h"

namespace Engine::UI 
{
    void UISystem::ToggleDebugger()
    {
    }

    void UISystem::ShowContextMenu(const Vector2& InPosition, std::function<void(ContextMenuItemBuilder&)> InBuildFunc)
    {
        if (m_contextMenu)
        {
            m_contextMenu->ShowAt(InPosition, InBuildFunc);
        }
    }

    void UISystem::HideContextMenu()
    {
        if (m_contextMenu)
        {
            m_contextMenu->Hide();
        }
    }

    bool UISystem::IsContextMenuOpen() const
    {
        //return false;
        return m_contextMenu ? m_contextMenu->IsOpen() : false;
    }

    void UISystem::CallOnWinCreate(IWindow *InWin, WindowContext InWinContext)
    {
        CreateContextFromWindowContext(InWinContext);
    }

    void UISystem::CallOnWinDestroy(IWindow *InWin, WindowContext InWinContext)
    {
        DestroyContextFromWindowContext(InWinContext);
    }

    void UISystem::CheckContextMenu(WindowContext InWinContext)
    {
        if (m_contextMenu == nullptr)
        {
            m_contextMenu = CreateRef<UIContextMenu>();
            m_contextMenu->OnInit();

            UIContext* ctx = GetContextFromWindowContext(InWinContext);
            if (ctx)
            {
                ctx->AddOverlayWidget(m_contextMenu);
            }
        }
    }

    bool UISystem::Initialize()
    {
        m_DH_OnWinCreate = Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinCreate().Subscribe(this, &UISystem::CallOnWinCreate);
        m_DH_OnWinDestroy = Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinDestroy().Subscribe(this, &UISystem::CallOnWinDestroy);

        return true;
    }

    void UISystem::Shutdown()
    {
        if (m_contextMenu)
        {
            m_contextMenu->Hide();
            m_contextMenu.reset();
        }
        Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinCreate().Unsubscribe(m_DH_OnWinCreate);
        Engine::EngineCore::GetEngineContext().GetWindowManager()->OnWinDestroy().Unsubscribe(m_DH_OnWinDestroy);
        m_ui_contexts.clear();
    }
    
    void UISystem::Update(float DeltaTime)
    {
        for (auto& [LKey, LVal] : m_ui_contexts)
        {
            if (!LVal) continue;
            LVal.get()->Update(DeltaTime);
        }
        if (m_contextMenu)
        {
            m_contextMenu->OnUpdate(DeltaTime);
        }
    }

    void UISystem::Render()
    {
        for (auto& [LKey, LVal] : m_ui_contexts)
        {
            if (!LVal) continue;
            LVal.get()->Render();
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

        CheckContextMenu(InContext);
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

        it->second.get()->Shutdown();

        m_ui_contexts.erase(InContext);
    }

    void UISystem::RegisterWidget(WindowContext InWinContext, TRef<UIElement> widget)
    {
        if (widget.get() == nullptr) return;

        UIContext* LCntxt = GetContextFromWindowContext(InWinContext);

        CheckContextMenu(InWinContext);

        if (LCntxt)
        {
            LCntxt->RegisterWidget(widget);
        }
    }

    void UISystem::RemoveWidget(WindowContext InWinContext, const TRef<UIElement> &widget)
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

    void UISystem::SetFocusWidget(WindowContext InWinContext, TRef<UIWidget> InNewFocus)
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

    void UISystem::SetHoverWidget(WindowContext InWinContext, TRef<UIWidget> InNewHover)
    {
        UIContext* LCntxt = GetContextFromWindowContext(InWinContext);
        if (LCntxt)
        {
            LCntxt->SetHoverWidget(InNewHover);
        }
    }
}
