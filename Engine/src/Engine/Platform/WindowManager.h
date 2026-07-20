// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Multiple inclusion guard for header file

#include "BaseEngine.h"                 // Base engine dependencies
#include <map>
#include <memory>                                   
#include "IWindow.h"

namespace Engine
{

    ADD_DELEGATE_THREE_PARAMS(DOnWinReSize, WindowContext, int,int)         // On window resize
    ADD_DELEGATE_ONE_PARAM(DOnWinFocusChange, WindowContext)                // On focus/minimize change
    ADD_DELEGATE_TWO_PARAMS(DOnWinCreate, TWeak<Engine::IWindow>, WindowContext)          // On create/delete window

    /**
     * @class WindowManager
     * @brief Controls all windows of the engine
     */
    class WindowManager
    {
        friend class EngineCore;
    public:
        //WindowManager() = default;
        virtual ~WindowManager() = default;
    //public:

        // Get and create
        virtual IWindow* GetEngineWindow(WindowContext InContext = WindowContext(0)) = 0;
        IWindow* GetEngineWindowActiv() {return GetEngineWindow(GetFocusWindow());};
        virtual WindowContext CreateEngineWindow(const struct WindowConfig InConfigWindow, bool InMainWindow = false) = 0;
        virtual void CloseEngineWindow(WindowContext InContext = WindowContext(0)) = 0;

        virtual WindowContext GetFocusWindow() const = 0;
        virtual void SetFocusWindow(WindowContext InFocus) = 0;
        virtual WindowContext GetMainWindow() const = 0;
        virtual void SetMainWindow(WindowContext InFocus) = 0;

        /**
         * @brief Update all window events 
         */
        virtual void PollEvents() = 0;

        /**
         * @brief Global frame update all windows
         */
        virtual void FrameUpdate(float InDeltaTime) = 0;

        /**
         * @brief Checks if the window should be closed (e.g., user clicked close button).
         * @return true if the window should be closed.
         */
        virtual bool ShouldClose() const = 0;
        /**
         * @brief Returns current window width.
         * @return Width in pixels.
         */
        int GetWidth(const WindowContext& InContext);
        /**
         * @brief Returns current window height.
         * @return Height in pixels.
         */
        int GetHeight(const WindowContext& InContext);
        /**
         * @brief Returns current window mode
         */
        WindowMode GetWindowMode(const WindowContext& InContext);
        /**
         * @brief Changes current window mode
         */
        void SetWindowMode(const WindowContext& InContext, WindowMode NewMode);

        //Delegate
        DOnWinReSize& OnWindowReSize() {return s_OnWindowReSize;}
        DOnWinFocusChange& OnHasFocusChange() {return s_OnHasFocusChange;}
        DOnWinFocusChange& OnMinimizedChange() {return s_OnMinimizedChange;}
        DOnWinCreate& OnWinCreate() {return s_OnWinCreate;}
        DOnWinCreate& OnWinDestroy() {return s_OnWinDestroy;}
    protected:
        DOnWinReSize s_OnWindowReSize;
        DOnWinFocusChange s_OnHasFocusChange;
        DOnWinFocusChange s_OnMinimizedChange;
        DOnWinCreate s_OnWinCreate;
        DOnWinCreate s_OnWinDestroy;
        int CountWindowID = 0;
    };
}

namespace std
{
    using namespace Engine;
    /*template<>
    struct hash<Engine::WindowContext>
    {
        size_t operator()(const Engine::WindowContext& ctx) const noexcept
        {
            return hash<uint8_t>{}(ctx.WindowID);
        }
    };*/
}