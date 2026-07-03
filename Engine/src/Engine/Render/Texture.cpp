// (c) Nikita Rogalev. All rights reserved.

#include "Texture.h"
#include "RendererAPI.h"

#include "Platform/RenderAPI/OpenGL/OpenGLTexture.h"
#include "Platform/RenderAPI/Vulkan/VulkanTexture.h"

namespace Engine::Render
{
    TRef<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(specification);
			case RendererAPI::API::Vulkan:  return CreateRef<VulkanTexture2D>(specification);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	TRef<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
			case RendererAPI::API::Vulkan:  return CreateRef<VulkanTexture2D>(path);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}