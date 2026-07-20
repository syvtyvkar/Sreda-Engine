#include "GameObject.h"
#include "Engine/Render/RenderAPIFactory.h"
#include "Engine/Render/Renderer.h"

Engine::GameObject::GameObject()
{
    m_VertexArray = RenderAPIFactory::CreateVertexArray();

	/*float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};*/



	float vertices[8 * 7] = //{Vertex, Color}
	{
		-1.f, +1.f, -1.f, 0.8f, 0.2f, 0.8f, 1.0f,
    	+1.f, +1.f, -1.f, 0.2f, 0.3f, 0.8f, 1.0f,
    	+1.f, -1.f, -1.f, 0.8f, 0.8f, 0.2f, 1.0f,
    	-1.f, -1.f, -1.f, 0.8f, 0.2f, 0.8f, 1.0f,
    	-1.f, +1.f, +1.f, 0.2f, 0.3f, 0.8f, 1.0f,
    	+1.f, +1.f, +1.f, 0.8f, 0.8f, 0.2f, 1.0f,
    	+1.f, -1.f, +1.f, 0.8f, 0.2f, 0.8f, 1.0f,
    	-1.f, -1.f, +1.f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	TRef<VertexBuffer> vertexBuffer = RenderAPIFactory::CreateVertexBufferFromVertex(vertices, sizeof(vertices));
	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3*12] = 
	{ 
		0, 1, 2,
		0, 2, 3,
		2, 1, 5,
		2, 5, 6,
		3, 2, 6,
		3, 6, 7,
		0, 3, 7,
		0, 7, 4,
		1, 0, 4,
		1, 4, 5,
		6, 5, 4,
		6, 4, 7,
	};


	TRef<IndexBuffer> indexBuffer = RenderAPIFactory::CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	/////////////////////////////////////////////
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
}

Engine::GameObject::~GameObject()
{
}

void Engine::GameObject::Render()
{
    Renderer::Submit(m_Shader, m_VertexArray);
}

void Engine::GameObject::Update(float InDeltaTime)
{
}
