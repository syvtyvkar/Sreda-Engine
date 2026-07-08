// (c) Nikita Rogalev. All rights reserved.

#include "Framebuffer.h"
#include "RendererAPI.h"
#include "RenderAPIFactory.h"

#include "Platform/RenderAPI/OpenGL/OpenGLFramebuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanFramebuffer.h"

namespace Engine::Render
{
    TRef<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanFramebuffer>(spec);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}