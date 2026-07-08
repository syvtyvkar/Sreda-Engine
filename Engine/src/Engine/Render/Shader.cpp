// (c) Nikita Rogalev. All rights reserved.

#include "Shader.h"

#include "BaseEngine.h"

#include "RendererAPI.h"
#include "RenderAPIFactory.h"

#include "Platform/RenderAPI/OpenGL/OpenGLShader.h"
#include "Platform/RenderAPI/Vulkan/VulkanShader.h"

namespace Engine::Render
{
    TRef<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLShader>(filepath);
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanShader>(filepath);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	TRef<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RenderAPIFactory::GetRenderAPI())
		{
			case RenderAPIFactory::RHI_API::None:    ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RenderAPIFactory::RHI_API::OpenGL:  return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
			case RenderAPIFactory::RHI_API::Vulkan:  return CreateRef<VulkanShader>(name, vertexSrc, fragmentSrc);
		}

		ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const TRef<Shader>& shader)
	{
		ENGINE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const TRef<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	TRef<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	TRef<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	TRef<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ENGINE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}