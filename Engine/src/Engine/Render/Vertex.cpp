// (c) Nikita Rogalev. All rights reserved.

#include "Vertex.h"
#include "RendererAPI.h"
#include "RenderAPIFactory.h"

#include "Platform/RenderAPI/OpenGL/OpenGLVertex.h"
#include "Platform/RenderAPI/Vulkan/VulkanVertex.h"

namespace Engine::Render
{
    TRef<VertexArray> VertexArray::Create()
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLVertexArray>();
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanVertexArray>();
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}