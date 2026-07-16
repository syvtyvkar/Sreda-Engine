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

EditorInstance::EditorInstance() : ApplicationInstance()
{
}

void EditorInstance::OnInit(Application* InOwnerApp)
{
    __super::OnInit(InOwnerApp);

    auto LConfig = Engine::EngineCore::GetEngineContext().GetConfigSystem();
    Engine::WindowConfig WindowConfig;
    WindowConfig.wight = LConfig->Get<int>("window.width", 800);
    WindowConfig.height = LConfig->Get<int>("window.height", 600);
    WindowConfig.title = GetNameApp();
    WindowConfig.vsync = LConfig->Get<bool>("render.vsync", false);
    WindowConfig.widget = std::make_shared<EditorMainWidget>();
    WindowContext LWinContx = EngineCore::GetEngineContext().GetWindowManager()->CreateEngineWindow(WindowConfig, true);

    IWindow* LWin = EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindow(LWinContx);

    Renderer::Init();

    m_ImGuiLayer.OnInit(InOwnerApp);
}

void EditorInstance::DeInit()
{
    m_ImGuiLayer.OnDeInit();

    auto LConfig = Engine::EngineCore::GetEngineContext().GetConfigSystem();
    LConfig->SaveConfig(EngineConfig::ENINGE_CONFIG_FILE);
    __super::DeInit();
}

void EditorInstance::OnStart()
{
    __super::OnStart();

    m_EditorRoot = CreateRef<UIEMain>();
    m_EditorRoot->OnInit();
}

void EditorInstance::OnEnd()
{
    __super::OnEnd();
}

void EditorInstance::Update(float DeltaTime)
{
    if (m_ImGuiLayer.IsInitialized())
        m_ImGuiLayer.BeginFrame();

    if (m_EditorRoot.get())
    {
        m_EditorRoot.get()->OnUpdate(DeltaTime);
    }
    __super::Update(DeltaTime);
}

void EditorInstance::OnRender()
{
    __super::OnRender();
}

void EditorInstance::OnRenderUI()
{
    if (!m_ImGuiLayer.IsInitialized()) return;

    if (m_EditorRoot.get())
    {
        m_EditorRoot.get()->OnRender();
    }

    m_ImGuiLayer.EndFrame();
}