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
