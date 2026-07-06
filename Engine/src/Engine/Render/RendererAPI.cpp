// (c) Nikita Rogalev. All rights reserved.

#include "RendererAPI.h"

#include "Platform/RenderAPI/OpenGL/OpenGLRendererAPI.h"
#include "Platform/RenderAPI/Vulkan/VulkanRendererAPI.h"

#include "Engine/Core/EngineConfig.h"

namespace Engine::Render
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	TUniquePtr<RendererAPI> RendererAPI::Create()
	{
        std::string renderAPIStr = EngineConfig::ConfigSystem::Get()->Get<std::string>("Engine.RenderAPI", "OpenGL");
        if (renderAPIStr == "Vulkan")
		{
			s_API = RendererAPI::API::Vulkan;
		}

        switch (s_API)
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLRendererAPI>();
			case RendererAPI::API::Vulkan:  return CreateUniquePtr<VulkanRendererAPI>();
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}