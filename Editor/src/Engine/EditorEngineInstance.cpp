#include "EditorEngineInstance.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Engine::Render;

EditorAppInstance::EditorAppInstance() : ApplicationInstance(), m_CameraController(1280.0f / 720.0f)
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
		});
		m_CameraController.OnResize(InOwnerApp->GetWindow()->GetWidth(),InOwnerApp->GetWindow()->GetHeight());
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
    m_EditorInputHotKey = std::make_unique<EditorInputComponent>();
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
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_FlatColorShader->Bind();

	m_AnimIs_x = m_AnimIs_x ? ((m_Anim_x >= 1.f) ? false : true) : ((m_Anim_x <= 0.f) ? true : false);
	m_Anim_x = m_AnimIs_x ? (m_Anim_x+DeltaTime*0.8f) : (m_Anim_x-DeltaTime*0.8f);
	m_AnimIs_y = m_AnimIs_y ? ((m_Anim_y >= 1.f) ? false : true) : ((m_Anim_y <= 0.f) ? true : false);
	m_Anim_y = m_AnimIs_y ? (m_Anim_y+(DeltaTime*0.3f)) : (m_Anim_y-(DeltaTime*0.3f));
	m_AnimIs_z = m_AnimIs_z ? ((m_Anim_z >= 1.f) ? false : true) : ((m_Anim_z <= 0.f) ? true : false);
	m_Anim_z = m_AnimIs_z ? (m_Anim_z+(DeltaTime*0.2f)) : (m_Anim_z-(DeltaTime*0.2f));

	m_FlatColorShader->SetFloat3("u_Color", { m_Anim_x,  m_Anim_y, m_Anim_z }); //m_SquareColor));

	/*for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
		}
	}*/
	
	auto textureShader = m_ShaderLibrary.Get("SimpleTexture");

	m_Texture->Bind();
	Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(-100.0f), glm::vec3(0.2f)));
	m_CheckerboardTexture->Bind();
	Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(100.f), glm::vec3(0.5f)));

	//glm::vec3 pos(1 * 0.11f, 1 * 0.11f, 0.0f);
	//glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
	//Renderer::Submit(textureShader, m_SquareVA, transform);

	// Triangle
	Renderer::Submit(m_Shader, m_VertexArray);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f - 1.f, y * 0.11f - 1.f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
		}
	}

	Renderer::EndScene();
}

void EditorAppInstance::OnRender()
{
}

void EditorAppInstance::OnRenderUI()
{
}
