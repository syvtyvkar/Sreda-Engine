#pragma once

#include "Engine/Core/Base/Layer.h"
#include <string>

namespace Engine
{
    class Application;
    struct WindowContext;
}

class ImGuiLayer : public Engine::Layer
{
public:
    ImGuiLayer();
    virtual ~ImGuiLayer() = default;

    void OnInit(Engine::Application* InApp);
    void OnDeInit() override;

    void BeginFrame();
    void EndFrame();

    void SetupImGuiStyle();

    bool IsInitialized() const { return m_Initialized; }

private:
    bool m_Initialized = false;
    void* m_WindowHandle = nullptr;
};
