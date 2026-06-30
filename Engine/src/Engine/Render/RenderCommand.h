// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "../../Engine/Core/Log.h"
#include "../../Engine/Core/Utilities/Types.h"
#include "RendererAPI.h"

namespace Engine::Render
{
    class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const TColor& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		static void DrawIndexed(const TRef<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		static void DrawLines(const TRef<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}

		static void SetDepthTest(bool enabled)
		{
			s_RendererAPI->SetDepthTest(enabled);
		}

        static void SetDepthWrite(bool enabled)
        {
            s_RendererAPI->SetDepthWrite(enabled);
        }

        static void EnableScissor(bool enabled)
        {
            s_RendererAPI->EnableScissor(enabled);
        }

        static void SetScissor(int x, int y, int width, int height)
        {
            s_RendererAPI->SetScissor(x, y, width, height);
        }
    private:
		static TUniquePtr<RendererAPI> s_RendererAPI;
	};
}
