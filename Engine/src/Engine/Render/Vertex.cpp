// (c) Nikita Rogalev. All rights reserved.

#include "Vertex.h"
#include "RendererAPI.h"

#include "Platform/RenderAPI/OpenGL/OpenGLVertex.h"
#include "Platform/RenderAPI/Vulkan/VulkanVertex.h"

namespace Engine::Render
{
    TRef<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
			case RendererAPI::API::Vulkan:  return CreateRef<VulkanVertexArray>();
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}