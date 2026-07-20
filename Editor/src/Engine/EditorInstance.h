#pragma once

#include <Engine.h>

#include "../Editor/Input/EditorInputComponent.h"
#include "Engine/Core/Utilities/Types.h"
#include "Engine/Render/Font/Font.h"
#include "ImGuiLayer.h"

#include <memory>
#include <functional>

#include "Engine/Render/Camera/EditorCamera.h"
#include "Engine/Render/Vertex.h"
#include "Engine/Render/Shader.h"
#include "Engine/Render/Texture.h"

//#include "Engine/Render/RenderGraph/RenderGraph.h"
//#include "Engine/Render/RenderGraph/Passes/GBufferPass.h"
//#include "Engine/Render/RenderGraph/Passes/LightingPass.h"
//#include "Engine/Render/RenderGraph/Passes/CompositePass.h"

#include "../Editor/UI/Framework/UIEditorBase.h"
#include "Engine/Scene/Scene.h"

using namespace Engine::Render;

class EditorInstance : public Engine::ApplicationInstance
{
public:
    EditorInstance();
    virtual ~EditorInstance() {};

    virtual void OnInit(class Application* InOwnerApp) override;
    virtual void DeInit() override;

    void OnStart() override;
    void OnEnd() override;
    virtual std::string GetNameApp() override { return "Editor"; }

    virtual void Update(float DeltaTime) override;
    virtual void OnRender() override;
    void OnRenderUI() override;

    //uint32_t GetViewportColorAttachmentID() const;
    //void ResizeViewport(uint32_t width, uint32_t height);

private:
    //void InitRenderGraph();
    //void RenderScene();

    TUniquePtr<EditorInputComponent> m_EditorInputHotKey;

    ImGuiLayer m_ImGuiLayer;
    TRef<UIEditorBase> m_EditorRoot;

    EditorCamera m_EditorCamera;

    //RenderGraph m_RenderGraph;
    //TRef<GBufferPass> m_GBufferPass;
    //TRef<LightingPass> m_LightingPass;
    //TRef<CompositePass> m_CompositePass;

    //TRef<Framebuffer> m_ViewportFramebuffer;
    uint32_t m_ViewportWidth = 800;
    uint32_t m_ViewportHeight = 600;
    bool m_ViewportDirty = false;

    TRef<Scene> m_Scene;

    Engine::DelegateHandle m_ResizeHandle;
};
