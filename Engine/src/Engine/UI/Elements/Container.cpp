#include "Container.h"

namespace Engine::UI 
{
    void UIContainer::OnRender()
    {
        __super::OnRender();
    }
    void UIContainer::OnUpdate(float deltaTime)
    {
        __super::OnUpdate(deltaTime);
    }
    void VerticalBox::ArrangeChildren()
    {
    }
    void HorizontalBox::ArrangeChildren()
    {
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