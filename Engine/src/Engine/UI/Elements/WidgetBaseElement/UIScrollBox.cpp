#include "UIScrollBox.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/RenderCommand.h"
#include "Engine/Input/Input.h"
#include <algorithm>

namespace Engine::UI
{
    using namespace Engine::Render;

    UIScrollBox::UIScrollBox()
    {
        SetFocusable(true);
    }

    void UIScrollBox::OnRender()
    {
        if (!IsVisible()) return;

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        Renderer2D::DrawQuad(pos + size * 0.5f, size, GetCurrentBackgroundColor());

        if (m_content)
        {
            RenderCommand::EnableScissor(true);
            RenderCommand::SetScissor((int)pos.x, (int)pos.y, (int)size.x, (int)size.y);

            Vector2 origPos = m_content->GetPosition();
            m_content->SetPosition({ -m_scrollX, -m_scrollY });
            m_content->OnRender();
            m_content->SetPosition(origPos);

            RenderCommand::EnableScissor(false);
        }

        if (m_showVertical) DrawScrollbarVertical();
        if (m_showHorizontal) DrawScrollbarHorizontal();

        UIWidget::OnRender();
    }

    void UIScrollBox::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;

        UpdateScrollableSize();
        ClampScroll();

        Vector2 mousePos = InputSystem::GetMousePosition();
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        bool inBounds = ContainsPoint(mousePos);

        if (inBounds)
        {
            float scrollDelta = (float)InputSystem::GetMouseScrollDelta();
            if (scrollDelta != 0.0f)
            {
                m_scrollY -= scrollDelta * m_scrollSpeed;
                ClampScroll();
            }
        }

        if (m_showVertical)
        {
            bool over = IsMouseOverScrollbarVertical(mousePos);
            if (over != m_hoveringVertical)
            {
                m_hoveringVertical = over;
            }

            if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseLeft) && over)
            {
                m_draggingVertical = true;
                m_dragStartScroll = m_scrollY;
                m_dragStartMouse = mousePos;
            }
        }

        if (m_showHorizontal)
        {
            bool over = IsMouseOverScrollbarHorizontal(mousePos);
            if (over != m_hoveringHorizontal)
            {
                m_hoveringHorizontal = over;
            }

            if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseLeft) && over)
            {
                m_draggingHorizontal = true;
                m_dragStartScroll = m_scrollX;
                m_dragStartMouse = mousePos;
            }
        }

        if (m_draggingVertical)
        {
            if (InputSystem::IsMouseButtonPressed(InputKey::MouseLeft))
            {
                Vector2 mouseDelta = InputSystem::GetMousePosition() - m_dragStartMouse;
                Vector2 size = GetComputedSize();
                float contentH = m_contentSize.y;
                float viewH = size.y;
                float maxScroll = std::max(0.0f, contentH - viewH);
                float thumbHeight = (viewH / std::max(contentH, viewH)) * (size.y - m_scrollbarWidth);
                float trackHeight = size.y - thumbHeight - m_scrollbarWidth;
                if (trackHeight > 0.0f)
                {
                    m_scrollY = m_dragStartScroll + (mouseDelta.y / trackHeight) * maxScroll;
                    ClampScroll();
                }
            }
            else
            {
                m_draggingVertical = false;
            }
        }

        if (m_draggingHorizontal)
        {
            if (InputSystem::IsMouseButtonPressed(InputKey::MouseLeft))
            {
                Vector2 mouseDelta = InputSystem::GetMousePosition() - m_dragStartMouse;
                Vector2 size = GetComputedSize();
                float contentW = m_contentSize.x;
                float viewW = size.x;
                float maxScroll = std::max(0.0f, contentW - viewW);
                float thumbWidth = (viewW / std::max(contentW, viewW)) * (size.x - m_scrollbarWidth);
                float trackWidth = size.x - thumbWidth - m_scrollbarWidth;
                if (trackWidth > 0.0f)
                {
                    m_scrollX = m_dragStartScroll + (mouseDelta.x / trackWidth) * maxScroll;
                    ClampScroll();
                }
            }
            else
            {
                m_draggingHorizontal = false;
            }
        }

        if (m_content)
        {
            m_content->OnUpdate(deltaTime);
        }
    }

    Vector2 UIScrollBox::GetComputedSize() const
    {
        return UIWidget::GetComputedSize();
    }

    void UIScrollBox::SetContent(TRef<UIElement> content)
    {
        if (m_content)
        {
            RemoveChild(m_content);
        }
        m_content = content;
        if (m_content)
        {
            AddChild(m_content);
        }
    }

    void UIScrollBox::SetScrollOffset(const Vector2& offset)
    {
        m_scrollX = offset.x;
        m_scrollY = offset.y;
        ClampScroll();
    }

    void UIScrollBox::SetScrollHorizontal(float scroll)
    {
        m_scrollX = scroll;
        ClampScroll();
    }

    void UIScrollBox::SetScrollVertical(float scroll)
    {
        m_scrollY = scroll;
        ClampScroll();
    }

    void UIScrollBox::UpdateScrollableSize()
    {
        if (!m_content) return;
        m_contentSize = m_content->GetComputedSize();
    }

    void UIScrollBox::ClampScroll()
    {
        if (!m_content) return;
        Vector2 size = GetComputedSize();
        float maxScrollX = std::max(0.0f, m_contentSize.x - size.x);
        float maxScrollY = std::max(0.0f, m_contentSize.y - size.y);
        m_scrollX = std::max(0.0f, std::min(m_scrollX, maxScrollX));
        m_scrollY = std::max(0.0f, std::min(m_scrollY, maxScrollY));
        m_showVertical = m_contentSize.y > size.y;
        m_showHorizontal = m_contentSize.x > size.x;
    }

    void UIScrollBox::DrawScrollbarVertical()
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        float sbWidth = m_scrollbarWidth;

        float barX = pos.x + size.x - sbWidth;
        float barY = pos.y;
        float barH = size.y;

        Renderer2D::DrawQuad({ barX + sbWidth * 0.5f, barY + barH * 0.5f }, { sbWidth, barH }, m_scrollbarBgColor);

        float contentH = m_contentSize.y;
        float viewH = size.y;
        float thumbHeight = (viewH / std::max(contentH, viewH)) * barH;
        float maxScroll = std::max(0.0f, contentH - viewH);
        float thumbY = barY;
        if (maxScroll > 0.0f)
        {
            thumbY = barY + (m_scrollY / maxScroll) * (barH - thumbHeight);
        }

        TColor thumbColor = m_hoveringVertical ? m_scrollbarThumbHoverColor : m_scrollbarThumbColor;
        Renderer2D::DrawQuad({ barX + sbWidth * 0.5f, thumbY + thumbHeight * 0.5f }, { sbWidth, thumbHeight }, thumbColor);
    }

    void UIScrollBox::DrawScrollbarHorizontal()
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        float sbWidth = m_scrollbarWidth;

        float barX = pos.x;
        float barY = pos.y + size.y - sbWidth;
        float barW = size.x;

        Renderer2D::DrawQuad({ barX + barW * 0.5f, barY + sbWidth * 0.5f }, { barW, sbWidth }, m_scrollbarBgColor);

        float contentW = m_contentSize.x;
        float viewW = size.x;
        float thumbWidth = (viewW / std::max(contentW, viewW)) * barW;
        float maxScroll = std::max(0.0f, contentW - viewW);
        float thumbX = barX;
        if (maxScroll > 0.0f)
        {
            thumbX = barX + (m_scrollX / maxScroll) * (barW - thumbWidth);
        }

        TColor thumbColor = m_hoveringHorizontal ? m_scrollbarThumbHoverColor : m_scrollbarThumbColor;
        Renderer2D::DrawQuad({ thumbX + thumbWidth * 0.5f, barY + sbWidth * 0.5f }, { thumbWidth, sbWidth }, thumbColor);
    }

    bool UIScrollBox::IsMouseOverScrollbarVertical(const Vector2& mousePos)
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        float sbWidth = m_scrollbarWidth;
        float barX = pos.x + size.x - sbWidth;
        return mousePos.x >= barX && mousePos.x <= barX + sbWidth &&
               mousePos.y >= pos.y && mousePos.y <= pos.y + size.y;
    }

    bool UIScrollBox::IsMouseOverScrollbarHorizontal(const Vector2& mousePos)
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        float sbWidth = m_scrollbarWidth;
        float barY = pos.y + size.y - sbWidth;
        return mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
               mousePos.y >= barY && mousePos.y <= barY + sbWidth;
    }
}
