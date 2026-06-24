// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include "Engine/UI/Framework/UIElement.h"

namespace Engine::UI 
{
    class UIContainer : public UIElement
    {
    public:
        void OnRender() override;
        void OnUpdate(float deltaTime) override;
        
        // Layout management
        virtual void ArrangeChildren() = 0;

        void SetSpacing(float spacing) { m_spacing = spacing; }
        float GetSpacing() const { return m_spacing; }
        
    protected:
        float m_spacing = 0.0f;
    };

    // Specific container implementations
    class VerticalBox : public UIContainer
    {
    public:
        void ArrangeChildren() override;
    };

    class HorizontalBox : public UIContainer
    {
    public:
        void ArrangeChildren() override;
    };

    class GridPanel : public UIContainer
    {
    public:
        void ArrangeChildren() override;
        void SetGridColumns(int columns);
        void SetGridRows(int rows);
    };
}
