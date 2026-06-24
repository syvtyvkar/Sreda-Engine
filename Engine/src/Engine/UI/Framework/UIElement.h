// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/Core/Utilities/Types.h"
#include "Engine/UI/Framework/UITypes.h"
#include <memory>
#include <unordered_map>

namespace Engine::UI 
{
    using namespace Engine;

    class UIElement : public std::enable_shared_from_this<UIElement>
    {
    public:
        virtual ~UIElement() = default;
        
        virtual void OnInit(){};

        virtual void OnRender();
        virtual void OnUpdate(float deltaTime);

        virtual void SetPosition(const Vector2& position) { m_position = position; }
        virtual Vector2 GetPosition() const { return m_position; }

        virtual void SetSize(const Vector2& size) { m_size = size; }
        virtual Vector2 GetSize() const { return m_size; }

        virtual Vector2 GetComputedPosition() const;
        virtual Vector2 GetComputedSize() const;

        void SetMargins(const UIMargins& margins) { m_margins = margins; }
        UIMargins GetMargins() const { return m_margins; }

        void SetPadding(const UIPadding& padding) { m_padding = padding; }
        UIPadding GetPadding() const { return m_padding; }

        void SetHorizontalAlignment(UIHorizontalAlignment align) { m_hAlignment = align; }
        UIHorizontalAlignment GetHorizontalAlignment() const { return m_hAlignment; }

        void SetVerticalAlignment(UIVerticalAlignment align) { m_vAlignment = align; }
        UIVerticalAlignment GetVerticalAlignment() const { return m_vAlignment; }

        void SetMinSize(const Vector2& minSize) { m_minSize = minSize; }
        Vector2 GetMinSize() const { return m_minSize; }

        void SetMaxSize(const Vector2& maxSize) { m_maxSize = maxSize; }
        Vector2 GetMaxSize() const { return m_maxSize; }

        void AddChild(TRef<UIElement> child);
        void RemoveChild(const TRef<UIElement>& child);
        const std::vector<TRef<UIElement>>& GetChildren() const { return m_children; }
        std::vector<TRef<UIElement>>& GetChildren() { return m_children; }

        bool IsVisible() const { return m_visible == UIVisibleMode::Visible || m_visible == UIVisibleMode::VisibleNoHit; }
        void SetVisible(UIVisibleMode visible) { m_visible = visible; }

        bool ContainsPoint(const Vector2& point) const;
        Vector2 GetAbsolutePosition() const { return GetComputedPosition(); }

        TRef<UIElement> GetParent() const { return m_parent.lock(); }

    protected:
        std::vector<TRef<UIElement>> m_children;
        TWeak<UIElement> m_parent;
        UIVisibleMode m_visible = UIVisibleMode::Visible;
        Vector2 m_position = { 0.0f, 0.0f };
        Vector2 m_size = { 30.0f, 30.0f };
        Vector2 m_minSize = { 0.0f, 0.0f };
        Vector2 m_maxSize = { FLT_MAX, FLT_MAX };

        UIMargins m_margins;
        UIPadding m_padding;
        UIHorizontalAlignment m_hAlignment = UIHorizontalAlignment::Left;
        UIVerticalAlignment m_vAlignment = UIVerticalAlignment::Top;
    };
}