// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include <memory>
#include <unordered_map>
#include <functional>
#include "Engine/Core/Utilities/Types.h"
#include "Engine/Platform/IWindow.h"
#include "Engine/UI/Framework/UIElement.h"

namespace Engine::UI 
{

    using namespace Engine;
    class UIContext 
    {
    public:
        virtual ~UIContext() = default; 
        

        virtual void InitContext(Window* window) = 0;

        // UI Element Management
        void RegisterWidget(TRef<UIElement> widget);
        void RemoveWidget(const TRef<UIElement>& widget);
    
        // Root element management
        void SetRootWidget(TRef<UIElement> root);
        TRef<UIElement> GetRootWidget() const { return m_rootWidget; }
        
        // Visibility
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual bool IsVisible() const = 0;

        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void BeginFrame()= 0;
	    virtual void EndFrame()= 0;

    protected:
        void RenderUIElements(TRef<UIElement> element);

        TRef<UIElement> m_rootWidget;
        std::vector<TRef<UIElement>> m_widgets;
    };


    /**
     * @class WindowAPIFactory
     * @brief Factory for creating a window instance depending on platform/API.
     *
     * Hides the details of the specific window implementation behind a unified Window interface.
     */
    class UIContextFactory 
    {
    public:
    /**
         * @brief Creates a unique pointer to a window object.
         * @return TUniquePtr<Window> Pointer to the created window.
         *
         * Depending on macros (e.g., ENGINE_WINDOW_GLFW) returns the corresponding
         * Window descendant (e.g., WindowGLF3). If no platform is defined,
         * may return nullptr.
         */
        static TUniquePtr<UIContext> create();
    };
}
