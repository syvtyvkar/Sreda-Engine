// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"
#include "RenderCommand.h"
#include "Camera/OrthographicCamera.h"
#include "Camera/EditorCamera.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "RenderAPIFactory.h"

namespace Engine::Render
{
    class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Submit(const TRef<Shader>& shader, const TRef<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RenderAPIFactory::RHI_API GetAPI() { return RenderAPIFactory::GetRenderAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static TUniquePtr<SceneData> s_SceneData;
	};
}
