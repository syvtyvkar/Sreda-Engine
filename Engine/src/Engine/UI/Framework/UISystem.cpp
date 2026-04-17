// (c) Nikita Rogalev. All rights reserved.

#include "UISystem.h"

namespace Engine::UI 
{
    void UISystem::ToggleDebugger()
    {

    }

    bool UISystem::Initialize(Window *window)
    {
        if (!CreateContext(window))
        {
            return false;
        }
        return true;
    }

    void UISystem::Shutdown()
    {
        DestroyContext();
    }
    
    void UISystem::Update()
    {
        if (m_context.get())
        {
            m_context.get()->Update();
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

    UIContext *UISystem::CreateContext(Window *window)
    {
        m_context = UIContextFactory::create();
        m_context.get()->InitContext(window);
        return m_context.get();
    }

    void UISystem::DestroyContext()
    {
        m_context.reset();
    }
}