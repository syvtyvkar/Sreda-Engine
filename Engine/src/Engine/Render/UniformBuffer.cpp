// (c) Nikita Rogalev. All rights reserved.

#include "UniformBuffer.h"
#include "RendererAPI.h"

#include "Platform/RenderAPI/OpenGL/OpenGLUniformBuffer.h"

namespace Engine::Render
{
    TRef<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}