// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <cstdint>
#include "Engine/Core/Utilities/Event.h"

namespace Engine
{

    /**
     * Abstract Input Key
     */

    enum class InputKey : uint32_t 
    {
        A = 0, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        D0 = 100, D1, D2, D3, D4, D5, D6, D7, D8, D9,

        F1 = 200, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        Escape = 300,
        Space,
        Tab,
        CapsLock,
        Enter,
        Backspace,
        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,

        Left = 400,
        Right,
        Up,
        Down,

        Shift = 500,
        Control,
        Alt,
        Super,

        PrintScreen = 600,
        ScrollLock,
        Pause,

        NumLock = 700,
        NumPad0, NumPad1, NumPad2, NumPad3, NumPad4,
        NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
        NumPadDecimal, NumPadDivide, NumPadMultiply,
        NumPadSubtract, NumPadAdd, NumPadEnter, NumPadEqual,

        Semicolon = 800,
        Equal,
        Comma,
        Minus,
        Period,
        Slash,
        Backtick,
        LeftBracket,
        Backslash,
        RightBracket,
        Quote,


        MouseLeft = 1000,
        MouseRight,
        MouseMiddle,
        Mouse4,
        Mouse5,

        Unknown = 0xFFFF,
        KeyCount  // Validation and Iteration
    };

    /**
     * Input States
     */
    enum class InputState : uint8_t 
    {
        Released = 0,
        Pressed,
        Repeated,
        Unknown
    };
}