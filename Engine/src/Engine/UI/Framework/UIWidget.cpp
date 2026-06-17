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

        DrawBackground();
        DrawBorder();
        DrawContent();

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

    void UIWidget::DrawBackground()
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        TColor color = GetCurrentBackgroundColor();

        Renderer2D::DrawQuad(pos, size, color);
    }

    void UIWidget::DrawBorder()
    {
        float bw = m_style.borderWidth;
        if (bw <= 0.0f) return;

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        TColor color = GetCurrentBorderColor();

        Renderer2D::DrawRect({ pos.x, pos.y, 0.0f }, size, color);
    }

    void UIWidget::DrawContent()
    {
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

    TColor UIWidget::GetCurrentTextColor() const
    {
        if (!m_enabled)
            return m_style.disabledTextColor;
        return m_style.textColor;
    }

    TColor UIWidget::GetCurrentBorderColor() const
    {
        if (m_hovered || m_pressed)
            return m_style.hoveredBorderColor;
        return m_style.borderColor;
    }
}