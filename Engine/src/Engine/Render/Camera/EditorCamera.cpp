// (c) Nikita Rogalev. All rights reserved.

#include "EditorCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Input/Input.h"

namespace Engine::Render
{
    EditorCamera::EditorCamera(float fov, float aspect, float nearPlane, float farPlane)
		: m_FOV(fov), m_AspectRatio(aspect), m_NearClip(nearPlane), m_FarClip(farPlane),
          m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane)),
          m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		RecalculateViewMatrix();
	}

    void EditorCamera::Update(float InDeltaTime)
    {
        if (!Engine::InputSystem::GetInstance()) return;

        if (!Engine::InputSystem::GetCursorVisible())
        {
            Vector2 mouseDelta = Engine::InputSystem::GetMouseDelta();
            m_Yaw += mouseDelta.x * m_MouseSensitivity;
            m_Pitch += mouseDelta.y * m_MouseSensitivity;      
            ENGINE_LOG_TRACE("EDITOR CAMERA Mouse Move: x {} y {}",mouseDelta.x,mouseDelta.y);   

            if (m_Pitch > 89.0f) m_Pitch = 89.0f;
            if (m_Pitch < -89.0f) m_Pitch = -89.0f;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        glm::vec3 forward = glm::normalize(front);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        float speed = m_MoveSpeed * InDeltaTime;
        if (Engine::InputSystem::IsKeyPressed(InputKey::Shift))
            speed *= m_SpeedMultiplier;

        if (Engine::InputSystem::IsKeyPressed(InputKey::W))
            m_Position += Vector3(forward * speed);
        if (Engine::InputSystem::IsKeyPressed(InputKey::S))
            m_Position -= Vector3(forward * speed);
        if (Engine::InputSystem::IsKeyPressed(InputKey::A))
            m_Position -= Vector3(right * speed);
        if (Engine::InputSystem::IsKeyPressed(InputKey::D))
            m_Position += Vector3(right * speed);
        if (Engine::InputSystem::IsKeyPressed(InputKey::Q))
            m_Position -= Vector3(up * speed);
        if (Engine::InputSystem::IsKeyPressed(InputKey::E))
            m_Position += Vector3(up * speed);

        RecalculateViewMatrix();
    }

    void EditorCamera::SetProjection(float fov, float aspect, float nearPlane, float farPlane)
	{
        m_FOV = fov;
        m_AspectRatio = aspect;
        m_NearClip = nearPlane;
        m_FarClip = farPlane;
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

    void EditorCamera::SetViewportSize(float width, float height)
    {
        if (height == 0.0f) return;
        m_AspectRatio = width / height;
        SetProjection(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
    }

	void EditorCamera::RecalculateViewMatrix()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		glm::vec3 forward = glm::normalize(front);

		m_ViewMatrix = glm::lookAt(m_Position.GetGlmVec3(), m_Position.GetGlmVec3() + forward, glm::vec3(0.0f, 1.0f, 0.0f));
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}