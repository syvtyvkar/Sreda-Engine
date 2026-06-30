// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/UI/Framework/IUIContext.h"
#include "Engine/UI/Framework/UIWidget.h"
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
        void Shutdown();

        virtual void InitContext() override;

        virtual void Show() override;
        virtual void Hide() override;
        virtual bool IsVisible() const override;

        virtual void Update(float DeltaTime) override;

    private:
        TRef<UIWidget> HitTest(TRef<UIElement> root, const Vector2& point);
        void HandleInput();

        OrthographicCamera m_ui_camera;
        bool m_bIsVisible = true;

        TRef<UIWidget> m_hoveredWidget;
        Vector2 m_lastMousePos;

        TRef<UIWidget> m_FocusWidget;
        TRef<UIWidget> m_LastPressWidget;

        Engine::DelegateHandle m_mousePressedHandle;
        Engine::DelegateHandle m_mouseReleasedHandle;
    };
}