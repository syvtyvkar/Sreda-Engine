// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Multiple inclusion guard for header file

#include "BaseEngine.h"                             // Base engine dependencies

#include <string>                                   // For std::string
#include <functional>                               // For std::function (used in resize callback)
#include <memory>                                   
#include "Engine/Render/GraphicsContext.h"          // Render graphics context
//#include "Engine/Scene/Scene.h"                   // Scene class containing game objects

namespace Engine
{

    ADD_DELEGATE_TWO_PARAMS(DOnWindowReSize, int,int)         // On window resize
    ADD_DELEGATE_ONE_PARAM(DOnWindowFocusChange, bool)        // On focus/minimize change
    ADD_DELEGATE_TWO_PARAMS(DOnWindowModeChange,WindowContext, WindowMode)   

    using namespace Engine::Render;

    /**
     * @class Window
     * @brief Abstract base class for platform-dependent window.
     *
     * Defines the interface for working with the application window: creation, update,
     * event handling, title changes, getting dimensions, etc.
     * Concrete implementations (e.g., for GLFW, SDL, WinAPI) should inherit this class.
     */
    class IWindow : public std::enable_shared_from_this<IWindow>
    {
    public:
        virtual ~IWindow()  = default;                                               // Virtual destructor for correct deletion of derived classes
        /**
         * @brief Initializes the window with the given configuration.
         * @param config Window parameters (width, height, title, etc.)
         * @return true on success, false on error.
         */
        virtual bool Init(const WindowConfig& config)= 0;
        /**
         * @brief Changes the window title.
         * @param NewName New title.
         */
        virtual void UpdateWindowName(std::string NewName)= 0;
        /**
         * @brief Checks if the window should be closed (e.g., user clicked close button).
         * @return true if the window should be closed.
         */
        virtual bool ShouldClose() const= 0;
        /**
         * @brief Prepare for rendering
         * Called every frame in the main application loop.
         */
        virtual void BeginRender()= 0;
        /**
         * @brief End of rendering
         * Called every frame in the main application loop.
         */
        virtual void EndRender()= 0;
        /**
         * @brief Updates window state: processes events, swaps buffers, etc.
         * Called every frame in the main application loop.
         */
        virtual void Update()= 0;
        /**
         * @brief Returns current window width.
         * @return Width in pixels.
         */
        virtual int GetWidth() const = 0;
        /**
         * @brief Returns current window height.
         * @return Height in pixels.
         */
        virtual int GetHeight() const = 0;
        /**
         * @brief Returns current window mode
         */
        virtual WindowMode GetWindowMode() = 0;
        /**
         * @brief Changes current window mode
         */
        virtual void SetWindowMode(WindowMode NewMode) = 0;
        /**
         * @brief Closes the window (sets ShouldClose flag to true).
         */
        virtual void Close()= 0;     
        /**
         * @brief Closes the window (sets ShouldClose flag to true).
         */
        virtual void WindowTerminate()= 0;     
        //virtual Scene* GetCurrentScene() = 0;
        virtual GraphicsContext* GetGraphicsContext() = 0;
        virtual void SetTittle(const std::string NewTittle) = 0;
        DOnWindowReSize& OnWindowReSize() {return s_OnWindowReSize;}
        DOnWindowFocusChange& OnHasFocusChange() {return s_OnHasFocusChange;}
        DOnWindowFocusChange& OnMinimizedChange() {return s_OnMinimizedChange;}
        DOnWindowModeChange& OnWindowModeChange() {return s_OnWindowModeChange;}

        virtual bool LoadIconFromFile() = 0;

        virtual void* GetWindowAPIHandle() = 0;

        virtual bool IsWindowHasFocus() = 0;
        virtual bool IsWindowMinimized() = 0;
        virtual void SetWindowHasFocus(bool InNewVal) = 0;
        virtual void SetWindowMinimized(bool InNewVal) = 0;
        virtual Engine::WindowContext GetWindowContext() = 0;
        virtual void FocusThisWindow() = 0;
        virtual Vector2 GetSizeWindow() const = 0;
        virtual Vector2 GetLocationWindow() const = 0;
        virtual void SetSizeWindow(Vector2 InNewVal) = 0;
        virtual void SetLocationWindow(Vector2 InNewVal) = 0;
    private:
        DOnWindowReSize s_OnWindowReSize;
        DOnWindowFocusChange s_OnHasFocusChange;
        DOnWindowFocusChange s_OnMinimizedChange;
        DOnWindowModeChange s_OnWindowModeChange;
    };
}
