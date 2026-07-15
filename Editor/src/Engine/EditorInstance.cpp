#include "EditorInstance.h"
#include "../Editor/Widgets/EditorMainWidget.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Core/EngineConfig.h"
#include "Engine/Render/RenderAPIFactory.h"


EditorInstance::EditorInstance() : ApplicationInstance()
{
}

void EditorInstance::OnInit(Application *InOwnerApp)
{
	__super::OnInit(InOwnerApp);

    auto LConfig = Engine::EngineCore::GetEngineContext().GetConfigSystem();                    // Get config system
    Engine::WindowConfig WindowConfig;                                                          // TODO: MOVE TO CONFIGS!!! Window parameter setup 
    WindowConfig.wight = LConfig->Get<int>("window.width", 800);
    WindowConfig.height = LConfig->Get<int>("window.height", 600);
    WindowConfig.title = GetNameApp();
    WindowConfig.vsync = LConfig->Get<bool>("render.vsync",false);
	WindowConfig.widget = std::make_shared<EditorMainWidget>();
    WindowContext LWinContx = EngineCore::GetEngineContext().GetWindowManager()->CreateEngineWindow(WindowConfig, true);

	IWindow* LWin = EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindow(LWinContx);

    Renderer::Init();
}

void EditorInstance::DeInit()
{
	auto LConfig = Engine::EngineCore::GetEngineContext().GetConfigSystem();                    // Get config system
	LConfig->SaveConfig(EngineConfig::ENINGE_CONFIG_FILE);
	__super::DeInit();
}

void EditorInstance::OnStart()
{
	__super::OnStart();
}

void EditorInstance::OnEnd()
{
    __super::OnEnd();
   
}

void EditorInstance::Update(float DeltaTime)
{
	__super::Update(DeltaTime);
}

void EditorInstance::OnRender()
{
	__super::OnRender();
}

void EditorInstance::OnRenderUI()
{
	ApplicationInstance::OnRenderUI();
}

void EditorInstance::CallOnWindowReSize(int x, int y)
{
	
}
