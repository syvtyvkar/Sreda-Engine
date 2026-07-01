// (c) Nikita Rogalev. All rights reserved.

#include "UISystem.h"
#include "UIBuilder.h"

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

    bool UISystem::Initialize()
    {
        if (!CreateContext())
        {
            return false;
        }
        //GetContext()->SetRootWidget(UIBuilder::CreateWidget());

        m_contextMenu = CreateUniquePtr<UIContextMenu>();
        return true;
    }

    void UISystem::Shutdown()
    {
        if (m_contextMenu)
        {
            m_contextMenu.get()->ClearAllButton();
            m_contextMenu.reset();
        }
        DestroyContext();
    }
    
    void UISystem::Update(float DeltaTime)
    {
        if (m_context.get())
        {
            m_context.get()->Update(DeltaTime);
        }
        if (m_contextMenu.get())
        {
            m_contextMenu.get()->OnUpdate(DeltaTime);
        }
    }

    void UISystem::Render()
    {
        if (m_context.get())
        {
            m_context.get()->Render();
        }
        if (m_contextMenu.get())
        {
            m_contextMenu.get()->OnRender();
        }
    }

    void UISystem::BeginFrame()
    {
        if (m_context.get())
        {
            m_context.get()->BeginFrame();
        }
    }

    void UISystem::EndFrame()
    {
        if (m_context.get())
        {
            m_context.get()->EndFrame();
        }
    }

    UIContext *UISystem::CreateContext()
    {
        m_context = UIContextFactory::create();
        ENGINE_ASSERT(m_context, "No UI context! The UI System cannot be initialized!");
        m_context.get()->InitContext();
        return m_context.get();
    }

    void UISystem::DestroyContext()
    {
        m_context.reset();
    }

    void UISystem::RegisterWidget(TRef<UIElement> widget)
    {
        if (widget.get() == nullptr) return;

        if (m_context.get() != nullptr)
        {
            m_context.get()->RegisterWidget(widget);
        }
    }

    void UISystem::RemoveWidget(const TRef<UIElement> &widget)
    {
        if (widget.get() == nullptr) return;
        
        if (m_context.get() != nullptr)
        {
            m_context.get()->RemoveWidget(widget);
        }
    }
    UIWidget *UISystem::GetFocusWidget()
    {
        if (GetContext())
        {
            return m_context.get()->GetFocusWidget();
        }
        return nullptr;
    }

    void UISystem::SetFocusWidget(TRef<UIWidget> InNewFocus)
    {
        if (GetContext())
        {
            return m_context.get()->SetFocusWidget(InNewFocus);
        }
    }

    UIWidget *UISystem::GetHoverWidget()
    {
        if (GetContext())
        {
            return m_context.get()->GetHoverWidget();
        }
        return nullptr;
    }

    void UISystem::SetHoverWidget(TRef<UIWidget> InNewHover)
    {
        if (GetContext())
        {
            return m_context.get()->SetHoverWidget(InNewHover);
        }
    }
}