// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

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
        
        // Обновление и рендер (вызывать в главном цикле)
        void Update();
        void Render();
        void BeginFrame();
	    void EndFrame();
        
        // Управление контекстами
        UIContext* CreateContext(Window *window);
        UIContext* GetContext() {return m_context.get();}
        void DestroyContext();
        
        // Отладка
        void ToggleDebugger();
    private:
        TUniquePtr<UIContext> m_context;
    };

}