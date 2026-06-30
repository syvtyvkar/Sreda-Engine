#include "UIElement.h"
#include <algorithm>
#include <cfloat>

namespace Engine::UI 
{
    void UIElement::OnRender()
    {
        if (!IsVisible()) return;

        for (auto& child : m_children)
        {
            child->OnRender();
        }
    }

    void UIElement::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;

        for (auto& child : m_children)
        {
            child->OnUpdate(deltaTime);
        }
    }

    Vector2 UIElement::GetComputedPosition() const
    {
        auto parent = m_parent.lock();
        if (!parent)
            return m_position;

        Vector2 parentPos = parent->GetComputedPosition();
        Vector2 parentSize = parent->GetComputedSize();

        float x = parentPos.x + m_margins.left + m_position.x;
        float y = parentPos.y + m_margins.top + m_position.y;

        switch (m_hAlignment)
        {
            case UIHorizontalAlignment::Center:
                x = parentPos.x + (parentSize.x - m_size.x) * 0.5f + m_position.x;
                break;
            case UIHorizontalAlignment::Right:
                x = parentPos.x + parentSize.x - m_size.x - m_margins.right + m_position.x;
                break;
            case UIHorizontalAlignment::Stretch:
                x = parentPos.x + m_margins.left + m_position.x;
                break;
            default:
                x = parentPos.x + m_margins.left + m_position.x;
                break;
        }

        switch (m_vAlignment)
        {
            case UIVerticalAlignment::Center:
                y = parentPos.y + (parentSize.y - m_size.y) * 0.5f + m_position.y;
                break;
            case UIVerticalAlignment::Bottom:
                y = parentPos.y + parentSize.y - m_size.y - m_margins.bottom + m_position.y;
                break;
            case UIVerticalAlignment::Stretch:
                y = parentPos.y + m_margins.top + m_position.y;
                break;
            default:
                y = parentPos.y + m_margins.top + m_position.y;
                break;
        }

        return { x, y };
    }

    Vector2 UIElement::GetComputedSize() const
    {
        Vector2 size = m_size;

        auto parent = m_parent.lock();
        if (parent)
        {
            Vector2 parentSize = parent->GetComputedSize();

            if (m_hAlignment == UIHorizontalAlignment::Stretch)
            {
                float stretchedW = parentSize.x - m_margins.left - m_margins.right;
                if (stretchedW > m_minSize.x)
                    size.x = (stretchedW < m_maxSize.x) ? stretchedW : m_maxSize.x;
            }

            if (m_vAlignment == UIVerticalAlignment::Stretch)
            {
                float stretchedH = parentSize.y - m_margins.top - m_margins.bottom;
                if (stretchedH > m_minSize.y)
                    size.y = (stretchedH < m_maxSize.y) ? stretchedH : m_maxSize.y;
            }
        }

        size.x = std::max(m_minSize.x, std::min(size.x, m_maxSize.x));
        size.y = std::max(m_minSize.y, std::min(size.y, m_maxSize.y));

        return size;
    }

    bool UIElement::ContainsPoint(const Vector2 &point) const
    {
        if (!IsVisible()) return false;
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        return point.x >= pos.x && point.x <= pos.x + size.x &&
               point.y >= pos.y && point.y <= pos.y + size.y;
    }

    void UIElement::AddChild(TRef<UIElement> child)
    {
        if (child)
        {
            m_children.push_back(child);
            child->m_parent = shared_from_this();
        }
    }

    void UIElement::RemoveChild(const TRef<UIElement> &child)
    {
        if (!child) return;

        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end())
        {
            (*it)->m_parent.reset();
            m_children.erase(it);
        }
    }
}
