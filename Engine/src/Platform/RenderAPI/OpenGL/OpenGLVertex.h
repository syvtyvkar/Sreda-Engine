#pragma once

#include "Engine/Render/Vertex.h"

namespace Engine::Render
{
    class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const TRef<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const TRef<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<TRef<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const TRef<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<TRef<VertexBuffer>> m_VertexBuffers;
		TRef<IndexBuffer> m_IndexBuffer;
	};
}