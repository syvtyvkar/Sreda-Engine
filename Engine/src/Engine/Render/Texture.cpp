// (c) Nikita Rogalev. All rights reserved.

#include "Texture.h"
#include "RendererAPI.h"
#include "RenderAPIFactory.h"

#include "Platform/RenderAPI/OpenGL/OpenGLTexture.h"
#include "Platform/RenderAPI/Vulkan/VulkanTexture.h"

namespace Engine::Render
{
    TRef<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLTexture2D>(specification);
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanTexture2D>(specification);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	TRef<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanTexture2D>(path);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}