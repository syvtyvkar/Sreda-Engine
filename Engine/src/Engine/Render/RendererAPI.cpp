// (c) Nikita Rogalev. All rights reserved.

#include "RendererAPI.h"

#include "Platform/RenderAPI/OpenGL/OpenGLRendererAPI.h"

namespace Engine::Render
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	TUniquePtr<RendererAPI> RendererAPI::Create()
	{
        switch (s_API)
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateUniquePtr<OpenGLRendererAPI>();
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}