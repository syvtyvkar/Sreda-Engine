// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"
#include "RendererAPI.h"

namespace Engine::Render
{
    class RenderCommand
	{
	public:
		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const TColor& color);
		static void Clear();
		static void DrawIndexed(const TRef<VertexArray>& vertexArray, uint32_t indexCount = 0);
		static void DrawLines(const TRef<VertexArray>& vertexArray, uint32_t vertexCount);
		static void SetLineWidth(float width);
		static void SetDepthTest(bool enabled);
        static void SetDepthWrite(bool enabled);
        static void EnableScissor(bool enabled);
        static void SetScissor(int x, int y, int width, int height);
    private:
		static TUniquePtr<RendererAPI> s_RendererAPI;
	};
}
