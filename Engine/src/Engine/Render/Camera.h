// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/Core/Types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine 
{

    // Типы движения камеры
    enum class CameraMovement 
    {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    // Типы проекции
    enum class ProjectionType 
    {
        Perspective,
        Orthographic
    };

    struct CameraConfig 
    {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        float yaw = -90.0f;
        float pitch = 0.0f;
        float moveSpeed = 2.5f;
        float mouseSensitivity = 0.1f;
        float zoom = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
        ProjectionType projectionType = ProjectionType::Perspective;
        float orthoSize = 10.0f;
        unsigned int screenWidth = 800;
        unsigned int screenHeight = 600;
    };

    class Camera 
    {
    public:
        Camera();
        Camera(const CameraConfig& config);
        ~Camera() = default;

        // === Матрицы ===
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        Matrix4 getViewMatrixM4() const;
        Matrix4 getProjectionMatrixM4() const;

        // === Обработка ввода ===
        void processKeyboard(CameraMovement direction, float deltaTime);
        void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
        void processMouseScroll(float yOffset);

        // === Обновление настроек ===
        void setScreenSize(unsigned int width, unsigned int height);
        void setProjectionType(ProjectionType type);
        void setPosition(const glm::vec3& position);

        // === Геттеры ===
        glm::vec3 getPosition() const { return m_position; }
        glm::vec3 getFront() const { return m_front; }
        float getZoom() const { return m_zoom; }
        float getYaw() const { return m_yaw; }
        float getPitch() const { return m_pitch; }

    private:
        // Позиция и направления
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        // Углы Эйлера
        float m_yaw;
        float m_pitch;

        // Параметры движения
        float m_moveSpeed;
        float m_mouseSensitivity;
        float m_zoom;

        // Проекция
        ProjectionType m_projectionType;
        float m_nearPlane;
        float m_farPlane;
        float m_orthoSize;
        unsigned int m_screenWidth;
        unsigned int m_screenHeight;

        // Внутренние методы
        void updateCameraVectors();
        void updateProjectionMatrix();
    };

}