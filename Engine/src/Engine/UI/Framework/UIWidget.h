// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/UI/Framework/UIElement.h"
#include "Engine/Core/Utilities/Types.h"
#include "Engine/Core/Utilities/Event.h"
#include <functional>

namespace Engine::UI
{
    using namespace Engine;

    ADD_MULTICAST_DELEGATE(DOnUIWidgetClick);
    ADD_MULTICAST_DELEGATE(DOnUIWidgetHoverBegin);
    ADD_MULTICAST_DELEGATE(DOnUIWidgetHoverEnd);
    ADD_MULTICAST_DELEGATE(DOnUIWidgetPressBegin);
    ADD_MULTICAST_DELEGATE(DOnUIWidgetPressEnd);

    struct UIWidgetStyle
    {
        TColor backgroundColor = TColor(0.2f, 0.2f, 0.2f, 1.0f);
        TColor hoveredBackgroundColor = TColor(0.3f, 0.3f, 0.3f, 1.0f);
        TColor pressedBackgroundColor = TColor(0.15f, 0.15f, 0.15f, 1.0f);
        TColor disabledBackgroundColor = TColor(0.1f, 0.1f, 0.1f, 1.0f);

        TColor textColor = TColor(1.0f, 1.0f, 1.0f, 1.0f);
        TColor disabledTextColor = TColor(0.5f, 0.5f, 0.5f, 1.0f);

        TColor borderColor = TColor(0.4f, 0.4f, 0.4f, 1.0f);
        TColor hoveredBorderColor = TColor(0.6f, 0.6f, 0.6f, 1.0f);

        float borderWidth = 1.0f;
        float cornerRadius = 0.0f;
    };

    class UIWidget : public UIElement
    {
    public:
        UIWidget();
        virtual ~UIWidget() = default;

        void OnRender() override;
        void OnUpdate(float deltaTime) override;

        bool IsEnabled() const { return m_enabled; }
        void SetEnabled(bool enabled);

        bool IsHovered() const { return m_hovered; }
        void SetHovered(bool hovered) { m_hovered = hovered; }
        bool IsPressed() const { return m_pressed; }
        void SetPressed(bool pressed) { m_pressed = pressed; }
        bool IsFocused() const { return m_focused; }

        void SetFocusable(bool focusable) { m_focusable = focusable; }
        bool IsFocusable() const { return m_focusable; }

        void SetStyle(const UIWidgetStyle& style) { m_style = style; }
        const UIWidgetStyle& GetStyle() const { return m_style; }
        UIWidgetStyle& GetStyle() { return m_style; }

        DOnUIWidgetClick& OnClick() { return m_onClick; }
        DOnUIWidgetHoverBegin& OnHoverBegin() { return m_onHoverBegin; }
        DOnUIWidgetHoverEnd& OnHoverEnd() { return m_onHoverEnd; }
        DOnUIWidgetPressBegin& OnPressBegin() { return m_onPressBegin; }
        DOnUIWidgetPressEnd& OnPressEnd() { return m_onPressEnd; }

    protected:
        virtual void DrawBackground();
        virtual void DrawBorder();
        virtual void DrawContent();

        TColor GetCurrentBackgroundColor() const;
        TColor GetCurrentBorderColor() const;

        bool m_enabled = true;
        bool m_hovered = false;
        bool m_pressed = false;
        bool m_focused = false;
        bool m_focusable = false;

        UIWidgetStyle m_style;

        DOnUIWidgetClick m_onClick;
        DOnUIWidgetHoverBegin m_onHoverBegin;
        DOnUIWidgetHoverEnd m_onHoverEnd;
        DOnUIWidgetPressBegin m_onPressBegin;
        DOnUIWidgetPressEnd m_onPressEnd;
    };
}