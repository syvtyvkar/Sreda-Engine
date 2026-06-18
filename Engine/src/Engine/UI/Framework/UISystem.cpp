// (c) Nikita Rogalev. All rights reserved.

#include "UISystem.h"
#include "UIBuilder.h"

namespace Engine::UI 
{
    void UISystem::ToggleDebugger()
    {

    }

    bool UISystem::Initialize()
    {
        if (!CreateContext())
        {
            return false;
        }
        //GetContext()->SetRootWidget(UIBuilder::CreateWidget());
        return true;
    }

    void UISystem::Shutdown()
    {
        DestroyContext();
    }
    
    void UISystem::Update(float DeltaTime)
    {
        if (m_context.get())
        {
            m_context.get()->Update(DeltaTime);
        }
    }

    void UISystem::Render()
    {
        if (m_context.get())
        {
            m_context.get()->Render();
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
}