// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"
#include "Buffer.h"
#include <unordered_map>
#include <memory>

namespace Engine::Render
{
    class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const TRef<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const TRef<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<TRef<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const TRef<IndexBuffer>& GetIndexBuffer() const = 0;

		static TRef<VertexArray> Create();
	};

}
