#include "Container.h"
#include <algorithm>

namespace Engine::UI 
{
    void UIContainer::OnRender()
    {
        ArrangeChildren();
        __super::OnRender();
    }

    void UIContainer::OnUpdate(float deltaTime)
    {
        ArrangeChildren();
        __super::OnUpdate(deltaTime);
    }

    Vector2 UIContainer::GetSize() const
    {
        if (GetParent().get() == nullptr) return m_size;

        Vector2 LSize = m_size;

        if (GetHorizontalAlignment() == UIHorizontalAlignment::Stretch)
        {
            LSize.x = GetParent().get()->GetSize().x;
        }

        if (GetVerticalAlignment() == UIVerticalAlignment::Stretch)
        {
            LSize.y = GetParent().get()->GetSize().y;
        }

        return LSize;
    }

    void VerticalBox::ArrangeChildren()
    {
        if (m_children.empty()) return;

        float totalFixed = 0.0f;
        int stretchCount = 0;

        for (auto& child : m_children)
        {
            if (child->GetVerticalAlignment() == UIVerticalAlignment::Stretch)
                stretchCount++;
            else
                totalFixed += child->GetSize().y;
        }

        float availableHeight = GetSize().y - m_padding.top - m_padding.bottom;
        float availableWidth = GetSize().x - m_padding.left - m_padding.right;
        float spacingTotal = m_spacing * std::max(0, (int)m_children.size() - 1);
        float stretchSpace = std::max(0.0f, availableHeight - totalFixed - spacingTotal);
        float stretchHeight = (stretchCount > 0) ? (stretchSpace / stretchCount) : 0.0f;

        float y = m_padding.top;
        for (auto& child : m_children)
        {
            child->SetPosition({ (float)m_padding.left, y });

            if (child->GetVerticalAlignment() == UIVerticalAlignment::Stretch)
            {
                float childW = (child->GetHorizontalAlignment() == UIHorizontalAlignment::Stretch)
                    ? availableWidth : child->GetSize().x;
                child->SetSize({ childW, stretchHeight });
            }
            else if (child->GetHorizontalAlignment() == UIHorizontalAlignment::Stretch)
            {
                child->SetSize({ availableWidth, child->GetSize().y });
            }

            y += child->GetComputedSize().y + m_spacing;
        }
    }

    void HorizontalBox::ArrangeChildren()
    {
        if (m_children.empty()) return;

        float totalFixed = 0.0f;
        int stretchCount = 0;

        for (auto& child : m_children)
        {
            if (child->GetHorizontalAlignment() == UIHorizontalAlignment::Stretch)
                stretchCount++;
            else
                totalFixed += child->GetSize().x;
        }

        float availableWidth = GetSize().x - m_padding.left - m_padding.right;
        float availableHeight = GetSize().y - m_padding.top - m_padding.bottom;
        float spacingTotal = m_spacing * std::max(0, (int)m_children.size() - 1);
        float stretchSpace = std::max(0.0f, availableWidth - totalFixed - spacingTotal);
        float stretchWidth = (stretchCount > 0) ? (stretchSpace / stretchCount) : 0.0f;

        float x = m_padding.left;
        for (auto& child : m_children)
        {
            x = x + child->GetPadding().left;
            break;
        }
        for (auto& child : m_children)
        {
            child->SetPosition({ x, (float)m_padding.top });

            if (child->GetHorizontalAlignment() == UIHorizontalAlignment::Stretch)
            {
                float childH = (child->GetVerticalAlignment() == UIVerticalAlignment::Stretch)
                    ? availableHeight : child->GetSize().y;
                child->SetSize({ stretchWidth, childH });
            }
            else if (child->GetVerticalAlignment() == UIVerticalAlignment::Stretch)
            {
                child->SetSize({ child->GetSize().x, availableHeight });
            }

            float LX = child->GetComputedSize().x;

            x += LX + m_spacing;
        }
    }

    void GridPanel::ArrangeChildren()
    {
    }
    void GridPanel::SetGridColumns(int columns)
    {
    }
    void GridPanel::SetGridRows(int rows)
    {
    }
}