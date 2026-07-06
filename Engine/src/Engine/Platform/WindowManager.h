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
    ADD_DELEGATE_TWO_PARAMS(DOnWinCreate, IWindow*, WindowContext)          // On create/delete window

    /**
     * @class WindowManager
     * @brief Controls all windows of the engine
     */
    class WindowManager
    {
    friend class EngineCore;
        std::unordered_map<WindowContext, TUniquePtr<IWindow>, WindowContextHash> m_windowList = {};
        WindowContext                                          m_focusWindow = WindowContext(0);
    public:
        WindowManager() = default;
        ~WindowManager();
    //public:

        // Get and create
        IWindow* GetEngineWindow(WindowContext InContext = WindowContext(0));
        WindowContext CreateEngineWindow(const struct WindowConfig InConfigWindow);
        void CloseEngineWindow(WindowContext InContext = WindowContext(0));
        WindowContext GetActivIWin() {return m_focusWindow;};

        /**
         * @brief Checks if the window should be closed (e.g., user clicked close button).
         * @return true if the window should be closed.
         */
        bool ShouldClose() const;
        /**
         * @brief Prepare for rendering
         * Called every frame in the main application loop.
         */
        void BeginRender();
        /**
         * @brief End of rendering
         * Called every frame in the main application loop.
         */
        void EndRender();
        /**
         * @brief Updates window state: processes events, swaps buffers, etc.
         * Called every frame in the main application loop.
         */
        void Update();
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
    private:
        DOnWinReSize s_OnWindowReSize;
        DOnWinFocusChange s_OnHasFocusChange;
        DOnWinFocusChange s_OnMinimizedChange;
        DOnWinCreate s_OnWinCreate;
        DOnWinCreate s_OnWinDestroy;
    public:
        void CallWindowReSize(WindowContext InContext, int InX,int InY);
        void CallWindowFocusChange(WindowContext InContext,bool InVal);
        void CallWindowMinimized(WindowContext InContext,bool InVal);
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