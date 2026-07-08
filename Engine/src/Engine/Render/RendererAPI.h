// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"
#include "Vertex.h"
#include <glm/glm.hpp>

namespace Engine::Render
{
    class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, Vulkan = 2
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const TColor& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const TRef<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const TRef<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		
		virtual void SetLineWidth(float width) = 0;

        virtual void SetDepthTest(bool enabled) = 0;
        virtual void SetDepthWrite(bool enabled) = 0;

        virtual void EnableScissor(bool enabled) = 0;
        virtual void SetScissor(int x, int y, int width, int height) = 0;

        /*static API GetAPI() { return s_API; }
        static void SetAPI(API api) { s_API = api; }
		static std::string GetNameAPI()
		{
			API LAPI = RenderAPIFactory::GetRenderAPI();
			switch (LAPI)
			{
			case API::OpenGL:
				return "OpenGL";
				break;
			case API::Vulkan:
				return "Vulkan";
				break;
			default:
				return "No Render API";
			}
		return "No Render API";
		}*/
	};
}
