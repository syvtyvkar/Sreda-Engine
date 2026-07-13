// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/UI/Framework/IUIContext.h"
#include "Engine/UI/Framework/UIWidget.h"
//#include "Engine/UI/Framework/UIRenderCommandList.h"
#include "Engine/Render/OrthographicCamera.h"
#include "Engine/Core/Utilities/Event.h"


namespace Engine::UI
{
    class UISredaContext : public UIContext
    {
    public:
        UISredaContext();
        virtual ~UISredaContext();

        bool Initialized = false;

        virtual void BeginFrame() override;
        virtual void EndFrame() override;
        virtual void Render() override;
        virtual void Shutdown() override;

        virtual void InitContext(Engine::WindowContext InContext) override;

        virtual void Show() override;
        virtual void Hide() override;
        virtual bool IsVisible() const override;

        virtual void Update(float DeltaTime) override;

        virtual Engine::WindowContext GetWindowContext() override {return m_WindowContext;};

        void CallOnWindowReSize(int x, int y); 

        //virtual UICommandList& GetCommandList() override { return m_uiCommandList; }

    protected:

    //Focus widget
        virtual UIWidget* GetFocusWidget() override {return m_FocusWidget.get();};
        virtual void SetFocusWidget(TRef<UIWidget> InNewFocus) override {m_FocusWidget = InNewFocus; };
        virtual UIWidget* GetHoverWidget() override {return m_hoveredWidget.get();};
        virtual void SetHoverWidget(TRef<UIWidget> InNewHover)override {m_hoveredWidget = InNewHover;};

    private:
        TRef<UIWidget> HitTest(TRef<UIElement> root, const Vector2& point);
        void HandleInput();

        void CollectCommands(TRef<UIElement> InElement);
        void SortCommand();

        OrthographicCamera m_ui_camera;
        bool m_bIsVisible = true;

        Vector2 m_lastMousePos;

        TRef<UIWidget> m_hoveredWidget;
        TRef<UIWidget> m_FocusWidget;
        TRef<UIWidget> m_LastPressWidget;

        Engine::DelegateHandle m_mousePressedHandle;
        Engine::DelegateHandle m_mouseReleasedHandle;

        WindowContext m_WindowContext = WindowContext(0);

        //UICommandList m_uiCommandList;
    };
}