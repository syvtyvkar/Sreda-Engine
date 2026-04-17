// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include "Engine/Core/Utilities/Types.h"
#include "Engine/UI/Framework/UITypes.h"
#include <memory>
#include <unordered_map>

namespace Engine::UI 
{
    using namespace Engine;


    class UIElement 
    {
    public:
        virtual ~UIElement() = default;
        
        virtual void OnRender();
        virtual void OnUpdate(float deltaTime);
        virtual void SetPosition(const Vector2& position) {m_position=position;}
        virtual void SetSize(const Vector2& size) {m_size=size;}
        
        // Parent-child relationship management
        void AddChild(std::shared_ptr<UIElement> child);
        void RemoveChild(const std::shared_ptr<UIElement>& child);
        
        bool IsVisible() const {return m_visible;}
        void SetVisible(bool InNewVis) {m_visible=InNewVis;}
    protected:
        std::vector<std::shared_ptr<UIElement>> m_children;
        std::weak_ptr<UIElement> m_parent;
        bool m_visible = true;
        Vector2 m_position = {0, 0};
        Vector2 m_size = {100, 30};
    };
}