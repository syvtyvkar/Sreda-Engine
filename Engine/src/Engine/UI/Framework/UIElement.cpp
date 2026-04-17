#include "UIElement.h"

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

    void UIElement::AddChild(std::shared_ptr<UIElement> child)
    {
        if (child != nullptr)
        {
            m_children.push_back(child);
            child->m_parent = std::make_shared<UIElement>(*this);
        }
    }
    void UIElement::RemoveChild(const std::shared_ptr<UIElement> &child)
    {
        if (child == nullptr) return;
    
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end())
        {
            (*it)->m_parent.reset();
            m_children.erase(it);
        }
    }
}
