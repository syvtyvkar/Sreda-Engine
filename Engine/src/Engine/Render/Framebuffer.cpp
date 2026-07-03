// (c) Nikita Rogalev. All rights reserved.

#include "Framebuffer.h"
#include "RendererAPI.h"

#include "Platform/RenderAPI/OpenGL/OpenGLFramebuffer.h"
#include "Platform/RenderAPI/Vulkan/VulkanFramebuffer.h"

namespace Engine::Render
{
    TRef<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
			case RendererAPI::API::Vulkan:  return CreateRef<VulkanFramebuffer>(spec);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}