// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include "Engine/Platform/IWindow.h"
#include "Engine/UI/Framework/IUIContext.h"
#include "Engine/Core/Utilities/Types.h"
#include <memory>
#include <unordered_map>
#include <functional>

namespace Engine::UI 
{
    class UISystem 
    {
    public:
        UISystem() = default;
        virtual ~UISystem() = default; 

        bool Initialize(Window* window);
        void Shutdown();
        
        // Update and render (call in main loop)
        void Update();
        void Render();
        void BeginFrame();
	    void EndFrame();
        
        // Context management
        UIContext* CreateContext(Window *window);
        UIContext* GetContext() {return m_context.get();}
        void DestroyContext();
        
        // Debug
        void ToggleDebugger();
    private:
        TUniquePtr<UIContext> m_context;
    };

}
