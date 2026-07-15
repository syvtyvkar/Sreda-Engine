#include "EditorEngineInstance.h"

#include "../Editor/Widgets/EditorMainWidget.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Core/EngineConfig.h"
#include "Engine/Render/RenderAPIFactory.h"

using namespace Engine::Render;

EditorAppInstance::EditorAppInstance() : ApplicationInstance(), m_CameraController(1280.0f / 720.0f), m_ui_camera(0.f,12800.f,720.f,0.f)
{
}

void EditorAppInstance::OnInit(Application *InOwnerApp)
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

	LWin->OnWindowReSize().Subscribe(this, &EditorAppInstance::CallOnWindowReSize);

	m_CameraController.OnResize(LWin->GetWidth(),LWin->GetHeight());

	int w = LWin->GetWidth();
	int h = LWin->GetHeight();
	m_ui_camera.SetProjection(0.0f, (float)w, (float)h, 0.0f);
	m_ui_camera.SetPosition(Vector3(0.0f));
	m_ui_camera.SetRotation(0.0f);

	Renderer::Init();
}

void EditorAppInstance::DeInit()
{
	auto LConfig = Engine::EngineCore::GetEngineContext().GetConfigSystem();                    // Get config system
	LConfig->SaveConfig(EngineConfig::ENINGE_CONFIG_FILE);
	__super::DeInit();
}

void EditorAppInstance::OnStart()
{

	/*if (glfwGetCurrentContext() == nullptr) 
	{
        ENGINE_LOG_CRITICAL("No OpenGL context current in thread {0}!", std::this_thread::get_id());
        return;
    }*/


    __super::OnStart();
    m_EditorInputHotKey = CreateUniquePtr<EditorInputComponent>();
    m_EditorInputHotKey.get()->Init();

	m_VertexArray = RenderAPIFactory::CreateVertexArray();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	TRef<VertexBuffer> vertexBuffer = RenderAPIFactory::CreateVertexBufferFromVertex(vertices, sizeof(vertices));
	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	TRef<IndexBuffer> indexBuffer = RenderAPIFactory::CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_SquareVA = RenderAPIFactory::CreateVertexArray();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	TRef<VertexBuffer> squareVB = RenderAPIFactory::CreateVertexBufferFromVertex(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	TRef<IndexBuffer> squareIB = RenderAPIFactory::CreateIndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	m_Shader = RenderAPIFactory::CreateShader("VertexPosColor", vertexSrc, fragmentSrc);

	std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

	m_FlatColorShader = RenderAPIFactory::CreateShader("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

	//auto textureShader = m_ShaderLibrary.Load("/Resources/Shaders/SimpleTexture.glsl");

	//m_Texture = RenderAPIFactory::CreateTexture2DFromPath("/Resources/Textures/t_test_texture_black.png");
	//m_CheckerboardTexture = RenderAPIFactory::CreateTexture2DFromPath("/Resources/Textures/SredaLogo.png");

	//textureShader->Bind();
	//textureShader->SetInt("u_Texture", 0);

	m_Font = FontManager::GetFontManager().GetFontDefault(); 
	if (!m_Font->GetAtlasTexture())
	{
		ENGINE_ASSERT(false, "Failed to load font!!!");
	}
}

void EditorAppInstance::OnEnd()
{
    __super::OnEnd();
    m_EditorInputHotKey.get()->DeInit();
    m_EditorInputHotKey.reset();
}

void EditorAppInstance::Update(float DeltaTime)
{
	__super::Update(DeltaTime);
    // Update
	m_CameraController.OnUpdate(DeltaTime);

	// Render
	RenderCommand::SetClearColor({ (int)m_Anim_x, (int)m_Anim_x, (int)m_Anim_x, 1 }); //0.01f
	RenderCommand::Clear();

	Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_FlatColorShader->Bind();

	float LD_x = DeltaTime*0.03f;
	m_AnimIs_x = m_AnimIs_x ? ((m_Anim_x >= 0.1f) ? false : true) : ((m_Anim_x <= 0.0f) ? true : false);
	m_Anim_x = m_AnimIs_x ? (m_Anim_x+LD_x) : (m_Anim_x-LD_x);
	float LD_y = DeltaTime*0.05f;
	m_AnimIs_y = m_AnimIs_y ? ((m_Anim_y >= 0.1f) ? false : true) : ((m_Anim_y <= 0.f) ? true : false);
	m_Anim_y = m_AnimIs_y ? (m_Anim_y+LD_y) : (m_Anim_y-LD_y);
	float LD_z = DeltaTime*0.03f;
	m_AnimIs_z = m_AnimIs_z ? ((m_Anim_z >= 0.2f) ? false : true) : ((m_Anim_z <= 0.1f) ? true : false);
	m_Anim_z = m_AnimIs_z ? (m_Anim_z+LD_z) : (m_Anim_z-LD_z);

	m_FlatColorShader->SetFloat3("u_Color", { m_Anim_x,  m_Anim_y, m_Anim_z }); //m_SquareColor));

	/*for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			Vector3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
		}
	}*/
	
	//auto textureShader = m_ShaderLibrary.Get("SimpleTexture");

	//m_Texture->Bind();
	//Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(-100.0f), glm::vec3(0.2f)));
 	//m_CheckerboardTexture->Bind();
	//Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(100.f), glm::vec3(0.2f)));

	// Triangle
	//Renderer::Submit(m_Shader, m_VertexArray);

	m_BoxesCount = (m_BoxesCount > 400) ? 0 : m_BoxesCount+1;

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			//if (x*y < m_BoxesCount || m_BoxesCount < 100 )
			if (rand() % 2)
			{
				Vector3 pos(x * 0.11f - 1.f, y * 0.11f - 1.f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos.GetGlmVec3()) * scale;
				//Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
	}
	Renderer::EndScene();

	//ENGINE_LOG_INFO("Atlas size: {}x{}, pixels size: {}", m_AtlasFont.width, m_AtlasFont.height, m_AtlasFont.pixels.size());
	//ENGINE_LOG_INFO("Font texture created, ID: {}", m_FontTexture->GetRendererID());
}

void EditorAppInstance::OnRender()
{
	__super::OnRender();
}

void EditorAppInstance::OnRenderUI()
{
	ApplicationInstance::OnRenderUI();
}

void EditorAppInstance::CallOnWindowReSize(int x, int y)
{
	m_CameraController.OnResize(x,y);
	m_ui_camera.SetProjection(0.0f, (float)x, (float)y, 0.0f);

	auto LConfig = Engine::EngineCore::GetEngineContext().GetConfigSystem();                    // Get config system
	
	LConfig->Set("window.width", x);
	LConfig->Set("window.height", y);
}
