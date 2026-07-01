// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard

#include "BaseEngine.h"
#include "OrthographicCamera.h"
#include <string>

using namespace Engine;

namespace Engine::Render
{
    class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio = 90.f, bool rotation = false);

		void OnUpdate(float ts);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		Vector3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};
}
