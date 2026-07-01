// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Multiple inclusion guard

#include "Engine/Platform/IWindow.h"
#include "Engine/UI/Framework/IUIContext.h"

#include "Engine/UI/Elements/WidgetBaseElement/UIContextMenu.h"

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

        bool Initialize();
        void Shutdown();
        
        // Update and render (call in main loop)
        void Update(float DeltaTime);
        void Render();
        void BeginFrame();
	    void EndFrame();
        
        // Context management
        UIContext* CreateContext();
        UIContext* GetContext() {return m_context.get();}
        void DestroyContext();

        // UI Element Management
        void RegisterWidget(TRef<UIElement> widget);
        void RemoveWidget(const TRef<UIElement>& widget);

        //Focus system
        UIWidget* GetFocusWidget();
        void SetFocusWidget(TRef<UIWidget> InNewFocus);
        UIWidget* GetHoverWidget();
        void SetHoverWidget(TRef<UIWidget> InNewHover);
        
        // Debug
        void ToggleDebugger();

        // ContextMenu
        bool AddButtonContext(std::string InNameButton, DOnUIWidgetClick& Handle);
        void RemoveButtonContext(std::string InNameButton);
        void ClearAllButton();
    private:
        TUniquePtr<UIContext> m_context;

        TUniquePtr<UIContextMenu> m_contextMenu;
    };

}
