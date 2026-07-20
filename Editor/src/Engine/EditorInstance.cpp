#include "EditorInstance.h"
#include "../Editor/Widgets/EditorMainWidget.h"

#include "imgui.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Core/EngineConfig.h"
#include "Engine/Render/RenderAPIFactory.h"

#include "../Editor/UI/Main/UIEMain.h"

EditorInstance::EditorInstance() : ApplicationInstance(),
    m_EditorCamera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f)
{

}

void EditorInstance::OnInit(Application* InOwnerApp)
{
    __super::OnInit(InOwnerApp);

    auto LConfig = Engine::EngineCore::GetEngineContext().GetConfigSystem();
    Engine::WindowConfig WindowConfig;
    WindowConfig.wight = LConfig->Get<int>("window.width", 800);
    WindowConfig.height = LConfig->Get<int>("window.height", 600);
    m_ViewportWidth = WindowConfig.wight;
    m_ViewportHeight = WindowConfig.height;
    WindowConfig.title = GetNameApp();
    WindowConfig.vsync = LConfig->Get<bool>("render.vsync", false);
    WindowConfig.widget = std::make_shared<EditorMainWidget>();
    WindowContext LWinContx = EngineCore::GetEngineContext().GetWindowManager()->CreateEngineWindow(WindowConfig, true);

    IWindow* LWin = EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindow(LWinContx);

    Renderer::Init();

    m_ImGuiLayer.OnInit(InOwnerApp);

    m_EditorCamera.SetViewportSize((float)m_ViewportWidth, (float)m_ViewportHeight);

    m_ResizeHandle = EngineCore::GetEngineContext().GetWindowManager()->OnWindowReSize().Subscribe(
        [this](WindowContext ctx, int w, int h)
        {
            m_ViewportWidth = w;
            m_ViewportHeight = h;
            m_EditorCamera.SetViewportSize((float)w, (float)h);
            Renderer::OnWindowResize(w, h);
        });
}

void EditorInstance::DeInit()
{
    m_ImGuiLayer.OnDeInit();

    EngineCore::GetEngineContext().GetWindowManager()->OnWindowReSize().Unsubscribe(m_ResizeHandle);

    auto LConfig = Engine::EngineCore::GetEngineContext().GetConfigSystem();
    LConfig->SaveConfig(EngineConfig::ENINGE_CONFIG_FILE);
    __super::DeInit();
}

void EditorInstance::OnStart()
{
    __super::OnStart();

    //m_EditorRoot = CreateRef<UIEMain>();
    //m_EditorRoot->OnInit();

   // auto* uiMain = static_cast<UIEMain*>(m_EditorRoot.get());
   // uiMain->SetEditorInstance(this);

    m_Scene = CreateRef<Scene>("NewScene");
    m_EditorInputHotKey = CreateUniquePtr<EditorInputComponent>();
    m_EditorInputHotKey.get()->Init();
}

void EditorInstance::OnEnd()
{
    __super::OnEnd();
    m_EditorInputHotKey.get()->DeInit();
    m_EditorInputHotKey.reset();
}

void EditorInstance::Update(float DeltaTime)
{
    //if (m_ImGuiLayer.IsInitialized())
    //    m_ImGuiLayer.BeginFrame();

    m_EditorCamera.Update(DeltaTime);

    //if (m_ViewportDirty)
    //{
    //    m_RenderGraph.OnResize(m_ViewportWidth, m_ViewportHeight);
    //    m_ViewportDirty = false;
    //}

    //if (m_EditorRoot.get())
    //{
    //    m_EditorRoot.get()->OnUpdate(DeltaTime);
    //}
    //__super::Update(DeltaTime);
}

void EditorInstance::OnRender()
{
    //RenderScene();
    __super::OnRender();

    RenderCommand::SetClearColor(TColor(0, 0, 0,255));
    RenderCommand::Clear();

    Renderer::BeginScene(m_EditorCamera);

	m_Scene.get()->Render();

	Renderer::EndScene();
}

void EditorInstance::OnRenderUI()
{
    //if (!m_ImGuiLayer.IsInitialized()) return;

    //if (m_EditorRoot.get())
    //{
    //    m_EditorRoot.get()->OnRender();
    //}

    //m_ImGuiLayer.EndFrame();
}

/*void EditorInstance::InitRenderGraph()
{
    FramebufferSpecification viewportSpec;
    viewportSpec.Width = m_ViewportWidth;
    viewportSpec.Height = m_ViewportHeight;
    viewportSpec.Samples = 1;
    viewportSpec.SwapChainTarget = false;
    viewportSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
    m_ViewportFramebuffer = RenderAPIFactory::CreateFramebuffer(viewportSpec);

    m_GBufferPass = CreateRef<GBufferPass>();
    m_LightingPass = CreateRef<LightingPass>();
    m_CompositePass = CreateRef<CompositePass>();

    m_CompositePass->SetViewportFramebuffer(m_ViewportFramebuffer);

    m_RenderGraph.AddPass(m_GBufferPass);
    m_RenderGraph.AddPass(m_LightingPass);
    m_RenderGraph.AddPass(m_CompositePass);

    m_RenderGraph.Build(m_ViewportWidth, m_ViewportHeight);
}

void EditorInstance::RenderScene()
{
    RenderPassContext context;
    context.ViewportWidth = m_ViewportWidth;
    context.ViewportHeight = m_ViewportHeight;

    auto& camera = m_CameraController.GetCamera();
    context.ViewProjection = camera.GetViewProjectionMatrix();

    glm::vec3 camPos(0.0f);
    context.CameraPosition[0] = camPos.x;
    context.CameraPosition[1] = camPos.y;
    context.CameraPosition[2] = camPos.z;

    RenderPassContext::LightData defaultLight;
    defaultLight.Position[0] = 2.0f;
    defaultLight.Position[1] = 2.0f;
    defaultLight.Position[2] = 2.0f;
    defaultLight.Color[0] = 1.0f;
    defaultLight.Color[1] = 1.0f;
    defaultLight.Color[2] = 1.0f;
    defaultLight.Intensity = 1.0f;
    context.Lights.push_back(defaultLight);

    context.RenderSceneCallback = [this]()
    {
        Renderer2D::BeginScene(m_CameraController.GetCamera());

        Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, TColor(0.8f, 0.2f, 0.2f));
        Renderer2D::DrawQuad({ 2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, TColor(0.2f, 0.8f, 0.2f));
        Renderer2D::DrawQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, TColor(0.2f, 0.2f, 0.8f));
        Renderer2D::DrawQuad({ 0.0f, 2.0f, 0.0f }, { 1.0f, 1.0f }, TColor(0.8f, 0.8f, 0.2f));
        Renderer2D::DrawQuad({ 0.0f, -2.0f, 0.0f }, { 1.0f, 1.0f }, TColor(0.8f, 0.2f, 0.8f));

        Renderer2D::EndScene();

        Renderer::BeginScene(m_CameraController.GetCamera());

        Renderer::

        Renderer::EndScene();
    };

    m_CompositePass->SetSceneTextureID(
        m_LightingPass->GetOutputFramebuffer()->GetColorAttachmentRendererID(0)
    );

    m_RenderGraph.Execute(context);
}

uint32_t EditorInstance::GetViewportColorAttachmentID() const
{
    if (m_ViewportFramebuffer)
        return m_ViewportFramebuffer->GetColorAttachmentRendererID(0);
    return 0;
}

void EditorInstance::ResizeViewport(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0) return;
    if (width == m_ViewportWidth && height == m_ViewportHeight) return;

    m_ViewportWidth = width;
    m_ViewportHeight = height;
    m_ViewportDirty = true;

    m_CameraController.OnResize((float)width, (float)height);
}*/
