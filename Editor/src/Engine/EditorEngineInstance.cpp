#include "EditorEngineInstance.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Engine::Render;

EditorAppInstance::EditorAppInstance() : ApplicationInstance(), m_CameraController(1280.0f / 720.0f), m_ui_camera(0.f,12800.f,0.,720.f)
{
}

void EditorAppInstance::OnInit(Application *InOwnerApp)
{
	__super::OnInit(InOwnerApp);

	if (InOwnerApp->GetWindow() != nullptr)
	{
		InOwnerApp->GetWindow()->OnWindowReSize().Subscribe([&,this](int x, int y) 
		{
			m_CameraController.OnResize(x,y);
			m_ui_camera.SetProjection(0.0f, (float)x, 0.0f, (float)y);
		});
		m_CameraController.OnResize(InOwnerApp->GetWindow()->GetWidth(),InOwnerApp->GetWindow()->GetHeight());

		int w = InOwnerApp->GetWindow()->GetWidth();
		int h = InOwnerApp->GetWindow()->GetHeight();
		m_ui_camera.SetProjection(0.0f, (float)w, 0.0f, (float)h);
		m_ui_camera.SetPosition(Vector3(0.0f));
		m_ui_camera.SetRotation(0.0f);
	}
}

void EditorAppInstance::DeInit()
{
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

	m_VertexArray = VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	TRef<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	TRef<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_SquareVA = VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	TRef<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	TRef<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
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

	m_Shader = Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

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

	m_FlatColorShader = Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

	auto textureShader = m_ShaderLibrary.Load("/Resources/Shaders/SimpleTexture.glsl");

	m_Texture = Texture2D::Create("/Resources/Textures/t_test_texture_black.png");
	m_CheckerboardTexture = Texture2D::Create("/Resources/Textures/SredaLogo.png");

	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);

	m_Font = CreateRef<Font>("Resources/Fonts/Cuprum.ttf", 48);
	if (!m_Font->GetAtlasTexture())
	{
		ENGINE_ASSERT(false, "Failed to load font!!!");
	}
	ENGINE_LOG_INFO("Font loaded, atlas texture ID={}", m_Font->GetAtlasTexture()->GetRendererID());

	GetOwnerApp()->GetWindow()->LoadIconFromFile();
}

void EditorAppInstance::OnEnd()
{
    __super::OnEnd();
    m_EditorInputHotKey.get()->DeInit();
    m_EditorInputHotKey.reset();
}

void EditorAppInstance::Update(float DeltaTime)
{
    // Update
	m_CameraController.OnUpdate(DeltaTime);

	// Render
	RenderCommand::SetClearColor({ (int)m_Anim_x, (int)m_Anim_x, (int)m_Anim_x, 1 }); //0.01f
	RenderCommand::Clear();

	Renderer::BeginScene(m_CameraController.GetCamera());

	//Renderer2D::RenderDrawText(L"Привет, мир!", m_FontTexture, m_AtlasFont.glyphs, 0.0f, 0.0f, 0.1f, glm::vec4(1.0f));
	//Renderer2D::RenderDrawText(L"Scale 2x", m_FontTexture, m_AtlasFont.glyphs, 0.0f, 0.0f, 0.1f, glm::vec4(1.0f));

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
	
	auto textureShader = m_ShaderLibrary.Get("SimpleTexture");

	m_Texture->Bind();
	Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(-100.0f), glm::vec3(0.2f)));
	m_CheckerboardTexture->Bind();
	Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(100.f), glm::vec3(0.5f)));

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

	Renderer2D::BeginScene(m_ui_camera);
	Renderer2D::RenderDrawText(L"Hello, world!", m_Font->GetAtlasTexture(), m_Font->GetGlyphs(), 100.0f, 100.0f, 1.0f, TColor::White);
	Renderer2D::RenderDrawText(L"Big text test", m_Font->GetAtlasTexture(), m_Font->GetGlyphs(), 100.0f, 200.0f, 2.0f, TColor::Orange);

	Renderer2D::DrawQuad(Vector2(0,500), Vector2(1500,100), m_Font->GetAtlasTexture());

	Renderer2D::EndScene();

	//ENGINE_LOG_INFO("Atlas size: {}x{}, pixels size: {}", m_AtlasFont.width, m_AtlasFont.height, m_AtlasFont.pixels.size());
	//ENGINE_LOG_INFO("Font texture created, ID: {}", m_FontTexture->GetRendererID());
}

void EditorAppInstance::OnRender()
{
}

void EditorAppInstance::OnRenderUI()
{
	//Renderer2D::BeginScene(m_CameraController.GetCamera());

	//Renderer2D::DrawCircle(glm::scale(glm::mat4(1.0f), Vector3(1.f)),{0.f,0.8f,0.6f,1.f},1.f);

	//Renderer2D::DrawText(L"Привет, мир!", m_FontTexture, m_AtlasFont.glyphs, 100.0f, 100.0f, 1.0f, glm::vec4(1.0f));
	//Renderer2D::DrawText(L"Scale 2x", m_FontTexture, m_AtlasFont.glyphs, 100.0f, 200.0f, 2.0f, glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));

	//Renderer2D::EndScene();
}
