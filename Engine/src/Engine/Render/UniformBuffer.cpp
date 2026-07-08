// (c) Nikita Rogalev. All rights reserved.

#include "UniformBuffer.h"
#include "RendererAPI.h"
#include "RenderAPIFactory.h"

#include "Platform/RenderAPI/OpenGL/OpenGLUniformBuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanUniformBuffer.h"

namespace Engine::Render
{
    TRef<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanUniformBuffer>(size, binding);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}