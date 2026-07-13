// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <string>

#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/Core/Utilities/Types.h"


namespace Engine
{
    /**
     * @struct WindowContext
     * @brief Structure containing window configuration parameters.
     */
    struct WindowContext
    {
        WindowContext(int InID) : WindowID(InID) {}
        WindowContext(const WindowContext& InContext) : WindowID(InContext.WindowID) {}
        int WindowID = 0;

        virtual void* GetWindowHande() {return nullptr; }

        /*size_t operator()(const Engine::WindowContext& ctx) const noexcept
        {
            return hash<int>{}(ctx.WindowID);
        }*/

        bool operator==(const WindowContext& InOther) const {return WindowID == InOther.WindowID;}
        bool operator!=(const WindowContext& InOther) const {return WindowID != InOther.WindowID;}
    };

    struct WindowContextHash 
    {
        size_t operator()(const WindowContext& ctx) const noexcept {return std::hash<uint8_t>{}(ctx.WindowID);}
    };

    /**
     * @struct WindowConfig
     * @brief Structure containing window configuration parameters.
     * 
     * TODO: MOVE TO CONFIGS!
     */
    struct WindowConfig
    {
        WindowConfig() {}
        WindowConfig(const WindowConfig& InConfig) : wight(InConfig.wight),height(InConfig.height),
        title(InConfig.title),resizable(InConfig.resizable),vsync(InConfig.vsync),cntx(InConfig.cntx),
        widget(InConfig.widget) {}

        int wight = 800;                                    // Window width in pixels
        int height = 600;                                   // Window height in pixels
        std::string title = "Render Window";                // Window title
        bool resizable = true;                              // Flag allowing the user to resize the window
        bool vsync = false;                                 // Vertical synchronization
        WindowContext cntx = WindowContext(0);              // Contex
        TRef<Engine::UI::UIWidget> widget = nullptr;  // Widget
    };

    /**
     * @struct WindowMode
     * @brief Window modes
     * 
     */
    enum class WindowMode : uint8_t 
    {
        Window = 0,
        Borderless = 1,
        Fullscreen = 2
    };
}