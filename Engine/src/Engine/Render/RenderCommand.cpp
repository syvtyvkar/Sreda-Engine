// (c) Nikita Rogalev. All rights reserved.

#include "RenderCommand.h"

#include "RenderAPIFactory.h"

namespace Engine::Render
{
    TUniquePtr<RendererAPI> RenderCommand::s_RendererAPI = nullptr;

    void RenderCommand::Init()
    {
        s_RendererAPI = RenderAPIFactory::CreateRendererAPI();
        if (s_RendererAPI)
            s_RendererAPI->Init();
    }

    void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        if (s_RendererAPI)
            s_RendererAPI->SetViewport(x, y, width, height);
    }

    void RenderCommand::SetClearColor(const TColor& color)
    {
        if (s_RendererAPI)
            s_RendererAPI->SetClearColor(color);
    }

    void RenderCommand::Clear()
    {
        if (s_RendererAPI)
            s_RendererAPI->Clear();
    }

    void RenderCommand::DrawIndexed(const TRef<VertexArray>& vertexArray, uint32_t indexCount)
    {
        if (s_RendererAPI)
            s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }

    void RenderCommand::DrawLines(const TRef<VertexArray>& vertexArray, uint32_t vertexCount)
    {
        if (s_RendererAPI)
            s_RendererAPI->DrawLines(vertexArray, vertexCount);
    }

    void RenderCommand::SetLineWidth(float width)
    {
        if (s_RendererAPI)
            s_RendererAPI->SetLineWidth(width);
    }

    void RenderCommand::SetDepthTest(bool enabled)
    {
        if (s_RendererAPI)
            s_RendererAPI->SetDepthTest(enabled);
    }

    void RenderCommand::SetDepthWrite(bool enabled)
    {
        if (s_RendererAPI)
            s_RendererAPI->SetDepthWrite(enabled);
    }

    void RenderCommand::EnableScissor(bool enabled)
    {
        if (s_RendererAPI)
            s_RendererAPI->EnableScissor(enabled);
    }

    void RenderCommand::SetScissor(int x, int y, int width, int height)
    {
        if (s_RendererAPI)
            s_RendererAPI->SetScissor(x, y, width, height);
    }
}
