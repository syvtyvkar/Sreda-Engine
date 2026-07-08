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

        void CreateContextFromWindowContext(WindowContext InContext);
        UIContext* GetContextFromWindowContext(WindowContext InContext);
        void DestroyContextFromWindowContext(WindowContext InContext);

        // UI Element Management
        void RegisterWidget(WindowContext InWinContext,TRef<UIElement> widget);
        void RemoveWidget(WindowContext InWinContext,const TRef<UIElement>& widget);

        //Focus system
        UIWidget* GetFocusWidget(WindowContext InWinContext);
        void SetFocusWidget(WindowContext InWinContext,TRef<UIWidget> InNewFocus);
        UIWidget* GetHoverWidget(WindowContext InWinContext);
        void SetHoverWidget(WindowContext InWinContext, TRef<UIWidget> InNewHover);
        
        // Debug
        void ToggleDebugger();

        // ContextMenu
        bool AddButtonContext(std::string InNameButton, DOnUIWidgetClick& Handle);
        void RemoveButtonContext(std::string InNameButton);
        void ClearAllButton();
    private:
        TUniquePtr<UIContextMenu> m_contextMenu;

        //Delegate
        Engine::DelegateHandle m_DH_OnWinCreate;
        Engine::DelegateHandle m_DH_OnWinDestroy;

        unordered_map<WindowContext,TRef<UIContext>, WindowContextHash> m_ui_contexts; 


        void CallOnWinCreate(IWindow* InWin, WindowContext InWinContext);
        void CallOnWinDestroy(IWindow* InWin, WindowContext InWinContext);
    };

}
