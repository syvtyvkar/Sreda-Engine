// (c) Nikita Rogalev. All rights reserved.

#pragma once // Защита от множественного включения

#include "Engine/Core/Utilities/Types.h"
#include <glm/glm.hpp>

namespace Engine::Render
{
    class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}