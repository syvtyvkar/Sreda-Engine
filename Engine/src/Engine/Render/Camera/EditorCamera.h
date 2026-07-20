// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"
#include "Camera.h"
#include <string>
#include <glm/glm.hpp>


namespace Engine::Render
{
    class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspect, float nearPlane, float farPlane);

		void Update(float InDeltaTime);

		void SetProjection(float fov, float aspect, float nearPlane, float farPlane);
		void SetViewportSize(float width, float height);

		const Vector3& GetPosition() const { return m_Position; }
		void SetPosition(const Vector3& position) { m_Position = position; RecalculateViewMatrix(); }

		Vector3 GetRotation() const { return m_Rotation; }
		void SetRotation(Vector3 rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
		void SetMouseSensitivity(float sens) { m_MouseSensitivity = sens; }
	private:
		void RecalculateViewMatrix();
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;


		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		Vector3 m_Position = { 0.0f, 0.0f, 3.0f };
		Vector3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		Vector3 m_Rotation = { 0.0f, 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = -90.0f;

		float m_MoveSpeed = 5.0f;
		float m_SpeedMultiplier = 3.0f;
		float m_MouseSensitivity = 0.2f;

		Vector2 m_ViewportWH;
	};
}