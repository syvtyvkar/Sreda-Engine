// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "../../Engine/Core/Log.h"
#include "../../Engine/Core/Utilities/Types.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include "RendererAPI.h"

namespace Engine::Render
{
    class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const TRef<Shader>& shader, const TRef<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static TUniquePtr<SceneData> s_SceneData;
	};
}
