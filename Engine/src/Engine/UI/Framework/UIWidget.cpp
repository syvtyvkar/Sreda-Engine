// (c) Nikita Rogalev. All rights reserved.

#include "UIWidget.h"
#include "Engine/Render/Renderer2D.h"

namespace Engine::UI
{
    using namespace Engine::Render;
    UIWidget::UIWidget()
    {
    }

    void UIWidget::OnRender()
    {
        if (!IsVisible()) return;

        UIElement::OnRender();
    }

    void UIWidget::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;

        UIElement::OnUpdate(deltaTime);
    }

    void UIWidget::SetEnabled(bool enabled)
    {
        if (m_enabled != enabled)
        {
            m_enabled = enabled;
            if (!m_enabled)
            {
                m_hovered = false;
                m_pressed = false;
            }
        }
    }

    TColor UIWidget::GetCurrentBackgroundColor() const
    {
        if (!m_enabled)
            return m_style.disabledBackgroundColor;
        if (m_pressed)
            return m_style.pressedBackgroundColor;
        if (m_hovered)
            return m_style.hoveredBackgroundColor;
        return m_style.backgroundColor;
    }

    TColor UIWidget::GetCurrentBorderColor() const
    {
        if (m_hovered || m_pressed)
            return m_style.hoveredBorderColor;
        return m_style.borderColor;
    }
}