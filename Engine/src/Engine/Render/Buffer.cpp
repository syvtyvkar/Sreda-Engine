// (c) Nikita Rogalev. All rights reserved.

#include "Buffer.h"
#include "RendererAPI.h"
#include "RenderAPIFactory.h"

#include "Platform/RenderAPI/OpenGL/OpenGLBuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanBuffer.h"

namespace Engine::Render
{
    TRef<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanVertexBuffer>(size);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	TRef<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size);
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanVertexBuffer>(vertices, size);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	TRef<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, size);
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanIndexBuffer>(indices, size);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}