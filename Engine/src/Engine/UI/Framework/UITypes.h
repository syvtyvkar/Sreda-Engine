// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

namespace Engine::UI 
{
    enum UIVisibleMode
    {
        Visible,
        VisibleNoHit,
        Hidden,
        Collapse
    };

    struct UIMargins
    {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };

    struct UIPadding
    {
        UIPadding() : left(0.f), top(0.f), right(0.f), bottom(0.f) {};
        UIPadding(float InLeft, float InTop,float InRight,float InBottom) : left(InLeft), top(InTop), right(InRight), bottom(InBottom) {};
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };

    enum class UIHorizontalAlignment
    {
        Left,
        Center,
        Right,
        Stretch
    };

    enum class UIVerticalAlignment
    {
        Top,
        Center,
        Bottom,
        Stretch
    };

    struct Border
    {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    };
}
