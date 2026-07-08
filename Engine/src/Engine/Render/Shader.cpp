// (c) Nikita Rogalev. All rights reserved.

#include "Shader.h"

#include "BaseEngine.h"

#include "RendererAPI.h"
#include "RenderAPIFactory.h"

#include "Platform/RenderAPI/OpenGL/OpenGLShader.h"
#include "Platform/RenderAPI/Vulkan/VulkanShader.h"

namespace Engine::Render
{
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
		auto shader = RenderAPIFactory::CreateShaderFromPath(filepath);
		Add(shader);
		return shader;
	}

	TRef<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = RenderAPIFactory::CreateShaderFromPath(filepath);
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