// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <GLFW/glfw3.h>
#include "Engine/Input/KeyCodes.h"

// TODO: Этому файлу нужен жесткий рефакторинг

namespace Input::GLFW 
{

#ifdef ENGINE_GLFW

    /**
     * Маппинг абстрактных Key → GLFW коды
     * 
     * Индекс массива = static_cast<uint32_t>(Key)
     * Значение = соответствующий GLFW_KEY_* или GLFW_MOUSE_BUTTON_*
     */
    inline constexpr int KeyToGLFW[] = {
        // === A-Z (0-25) ===
        GLFW_KEY_A, GLFW_KEY_B, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E,
        GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_I, GLFW_KEY_J,
        GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_M, GLFW_KEY_N, GLFW_KEY_O,
        GLFW_KEY_P, GLFW_KEY_Q, GLFW_KEY_R, GLFW_KEY_S, GLFW_KEY_T,
        GLFW_KEY_U, GLFW_KEY_V, GLFW_KEY_W, GLFW_KEY_X, GLFW_KEY_Y,
        GLFW_KEY_Z,

        // === D0-D9 (100-109) ===
        GLFW_KEY_0, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4,
        GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9,

        // === F1-F12 (200-211) ===
        GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5,
        GLFW_KEY_F6, GLFW_KEY_F7, GLFW_KEY_F8, GLFW_KEY_F9, GLFW_KEY_F10,
        GLFW_KEY_F11, GLFW_KEY_F12,

        // === Спецклавиши (300+) ===
        GLFW_KEY_ESCAPE,      // 300
        GLFW_KEY_SPACE,       // 301
        GLFW_KEY_TAB,         // 302
        GLFW_KEY_CAPS_LOCK,   // 303
        GLFW_KEY_ENTER,       // 304
        GLFW_KEY_BACKSPACE,   // 305
        GLFW_KEY_INSERT,      // 306
        GLFW_KEY_DELETE,      // 307
        GLFW_KEY_HOME,        // 308
        GLFW_KEY_END,         // 309
        GLFW_KEY_PAGE_UP,     // 310
        GLFW_KEY_PAGE_DOWN,   // 311

        // === Стрелки (400-403) ===
        GLFW_KEY_LEFT,        // 400
        GLFW_KEY_RIGHT,       // 401
        GLFW_KEY_UP,          // 402
        GLFW_KEY_DOWN,        // 403

        // === Модификаторы (500-503) ===
        GLFW_KEY_LEFT_SHIFT,  // 500
        GLFW_KEY_LEFT_CONTROL,// 501
        GLFW_KEY_LEFT_ALT,    // 502
        GLFW_KEY_LEFT_SUPER,  // 503

        // === Навигация (600-602) ===
        GLFW_KEY_PRINT_SCREEN, // 600
        GLFW_KEY_SCROLL_LOCK,  // 601
        GLFW_KEY_PAUSE,        // 602

        // === NumPad (700-715) ===
        GLFW_KEY_NUM_LOCK,     // 700
        GLFW_KEY_KP_0, GLFW_KEY_KP_1, GLFW_KEY_KP_2, GLFW_KEY_KP_3, GLFW_KEY_KP_4,
        GLFW_KEY_KP_5, GLFW_KEY_KP_6, GLFW_KEY_KP_7, GLFW_KEY_KP_8, GLFW_KEY_KP_9,
        GLFW_KEY_KP_DECIMAL, GLFW_KEY_KP_DIVIDE, GLFW_KEY_KP_MULTIPLY,
        GLFW_KEY_KP_SUBTRACT, GLFW_KEY_KP_ADD, GLFW_KEY_KP_ENTER, GLFW_KEY_KP_EQUAL,

        // === Знаки препинания (800-810) ===
        GLFW_KEY_SEMICOLON,    // 800
        GLFW_KEY_EQUAL,        // 801
        GLFW_KEY_COMMA,        // 802
        GLFW_KEY_MINUS,        // 803
        GLFW_KEY_PERIOD,       // 804
        GLFW_KEY_SLASH,        // 805
        GLFW_KEY_GRAVE_ACCENT, // 806
        GLFW_KEY_LEFT_BRACKET, // 807
        GLFW_KEY_BACKSLASH,    // 808
        GLFW_KEY_RIGHT_BRACKET,// 809
        GLFW_KEY_APOSTROPHE,   // 810

        // === Мышь (1000-1004) ===
        GLFW_MOUSE_BUTTON_LEFT,   // 1000
        GLFW_MOUSE_BUTTON_RIGHT,  // 1001
        GLFW_MOUSE_BUTTON_MIDDLE, // 1002
        GLFW_MOUSE_BUTTON_4,      // 1003
        GLFW_MOUSE_BUTTON_5,      // 1004
    };

    /**
     * Обратный маппинг: GLFW код → абстрактная Key
     */
    inline InputKey GLFWToEngineKey(int glfwKey) 
    {
        constexpr size_t size = sizeof(KeyToGLFW) / sizeof(KeyToGLFW[0]);
        for (size_t i = 0; i < size; ++i) 
        {
            if (KeyToGLFW[i] == glfwKey) 
            {
                return static_cast<InputKey>(static_cast<uint32_t>(InputKey::A) + i);
            }
        }
        // Обработка клавиш, которых нет в маппинге (например, правые модификаторы)
        if (glfwKey >= GLFW_KEY_A && glfwKey <= GLFW_KEY_Z) 
        {
            return static_cast<InputKey>(static_cast<uint32_t>(InputKey::A) + (glfwKey - GLFW_KEY_A));
        }
        if (glfwKey >= GLFW_KEY_0 && glfwKey <= GLFW_KEY_9) 
        {
            return static_cast<InputKey>(static_cast<uint32_t>(InputKey::D0) + (glfwKey - GLFW_KEY_0));
        }
        return InputKey::Unknown;
    }

    inline int EngineKeyToGLFW(InputKey key) 
    {
        auto idx = static_cast<uint32_t>(key);
        constexpr size_t size = sizeof(KeyToGLFW) / sizeof(KeyToGLFW[0]);
        
        // Прямой доступ для известных диапазонов
        if (idx >= static_cast<uint32_t>(InputKey::A) && idx <= static_cast<uint32_t>(InputKey::Z)) 
        {
            return KeyToGLFW[idx];
        }
        if (idx >= static_cast<uint32_t>(InputKey::MouseLeft) && idx <= static_cast<uint32_t>(InputKey::Mouse5)) 
        {
            return KeyToGLFW[idx];
        }
        
        // Безопасная проверка границ
        if (idx < size) {
            return KeyToGLFW[idx];
        }
        return GLFW_KEY_UNKNOWN;
    }

    static InputKey GLFWKeyToEngineKey(int glfwKey) 
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(InputKey::KeyCount); ++i) 
        {
            if (i < sizeof(KeyToGLFW) / sizeof(KeyToGLFW[0]) && KeyToGLFW[i] == glfwKey) 
            {
                 return static_cast<InputKey>(i);
            }
          }
        return InputKey::Unknown;
    }

#endif

}