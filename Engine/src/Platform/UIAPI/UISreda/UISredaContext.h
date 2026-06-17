// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include "Engine/UI/Framework/IUIContext.h"

namespace Engine::UI
{
    class UISredaContext : public UIContext
    {
    public:
        UISredaContext() = default;
        virtual ~UISredaContext() = default;

        bool Initialized = false;

        virtual void BeginFrame() override;
        virtual void EndFrame() override;
        virtual void Render() override;
        void Shutdown();

        virtual void InitContext(Engine::Window* window) override;

        // UIContext
        virtual void Show() override;
        virtual void Hide() override;
        virtual bool IsVisible() const override;

        virtual void Update() override;
    private:
    
    };
}