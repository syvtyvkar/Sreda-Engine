// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include "Engine/UI/Framework/IUIContext.h"
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/Render/OrthographicCamera.h"


namespace Engine::UI
{
    class UISredaContext : public UIContext
    {
    public:
        UISredaContext();
        virtual ~UISredaContext() = default;

        bool Initialized = false;

        virtual void BeginFrame() override;
        virtual void EndFrame() override;
        virtual void Render() override;
        void Shutdown();

        virtual void InitContext() override;

        // UIContext
        virtual void Show() override;
        virtual void Hide() override;
        virtual bool IsVisible() const override;

        virtual void Update(float DeltaTime) override;
    private:
        OrthographicCamera m_ui_camera;
        bool m_bIsVisible = true;
    };
}