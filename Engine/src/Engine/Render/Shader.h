// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "Engine/Core/Utilities/Types.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Engine/Core/FileSystem/AssetManager.h"

namespace Engine::Render
{
    class Shader : public IAsset
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const Vector2& value) = 0;
		virtual void SetFloat3(const std::string& name, const Vector3& value) = 0;
		virtual void SetFloat4(const std::string& name, const Vector4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const TRef<Shader>& shader);
		void Add(const TRef<Shader>& shader);
		TRef<Shader> Load(const std::string& filepath);
		TRef<Shader> Load(const std::string& name, const std::string& filepath);

		TRef<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, TRef<Shader>> m_Shaders;
	};
}
