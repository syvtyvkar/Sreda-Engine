// (c) Nikita Rogalev. All rights reserved.

#include "GraphicsContext.h"
#include "RendererAPI.h"

#include "Platform/RenderAPI/OpenGL/OpenGLContext.h"
#include "Platform/RenderAPI/Vulkan/VulkanContext.h"

namespace Engine::Render
{
    TUniquePtr<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLContext>(static_cast<GLFWwindow*>(window));
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanContext>(static_cast<GLFWwindow*>(window));
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}