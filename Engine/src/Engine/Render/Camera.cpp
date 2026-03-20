// (c) Nikita Rogalev. All rights reserved.

#include "Camera.h"
#include <algorithm>

namespace Engine 
{

    Camera::Camera() : Camera(CameraConfig()) {}

    Camera::Camera(const CameraConfig& config)
        : m_position(config.position)
        , m_worldUp(config.up)
        , m_yaw(config.yaw)
        , m_pitch(config.pitch)
        , m_moveSpeed(config.moveSpeed)
        , m_mouseSensitivity(config.mouseSensitivity)
        , m_zoom(config.zoom)
        , m_projectionType(config.projectionType)
        , m_nearPlane(config.nearPlane)
        , m_farPlane(config.farPlane)
        , m_orthoSize(config.orthoSize)
        , m_screenWidth(config.screenWidth)
        , m_screenHeight(config.screenHeight)
    {
        updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix() const 
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    glm::mat4 Camera::getProjectionMatrix() const 
    {
        if (m_projectionType == ProjectionType::Perspective) 
        {
            return glm::perspective(glm::radians(m_zoom), 
                                    (float)m_screenWidth / (float)m_screenHeight, 
                                    m_nearPlane, m_farPlane);
        } 
        else 
        {
            float aspect = (float)m_screenWidth / (float)m_screenHeight;
            return glm::ortho(-m_orthoSize * aspect, m_orthoSize * aspect, 
                             -m_orthoSize, m_orthoSize, 
                             m_nearPlane, m_farPlane);
        }
    }

    Matrix4 Camera::getViewMatrixM4() const
    {
        //return glm::lookAt(m_position, m_position + m_front, m_up);
        return Matrix4();
    }

    Matrix4 Camera::getProjectionMatrixM4() const
    {
        /*if (m_projectionType == ProjectionType::Perspective) {
            return glm::perspective(glm::radians(m_zoom), 
                                    (float)m_screenWidth / (float)m_screenHeight, 
                                    m_nearPlane, m_farPlane);
        } else {
            float aspect = (float)m_screenWidth / (float)m_screenHeight;
            return glm::ortho(-m_orthoSize * aspect, m_orthoSize * aspect, 
                             -m_orthoSize, m_orthoSize, 
                             m_nearPlane, m_farPlane);
        }*/
        return Matrix4();
    }

    void Camera::processKeyboard(CameraMovement direction, float deltaTime) 
    {
        float velocity = m_moveSpeed * deltaTime;

        switch (direction) {
            case CameraMovement::Forward:
                m_position += m_front * velocity;
                break;
            case CameraMovement::Backward:
                m_position -= m_front * velocity;
                break;
            case CameraMovement::Left:
                m_position -= m_right * velocity;
                break;
            case CameraMovement::Right:
                m_position += m_right * velocity;
                break;
            case CameraMovement::Up:
                m_position += m_worldUp * velocity;
                break;
            case CameraMovement::Down:
                m_position -= m_worldUp * velocity;
                break;
        }
    }

    void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) 
    {
        xOffset *= m_mouseSensitivity;
        yOffset *= m_mouseSensitivity;

        m_yaw += xOffset;
        m_pitch += yOffset;

        // Ограничиваем pitch чтобы камера не перевернулась
        if (constrainPitch) 
        {
            m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
        }

        updateCameraVectors();
    }

    void Camera::processMouseScroll(float yOffset) 
    {
        m_zoom -= yOffset;
        m_zoom = std::clamp(m_zoom, 1.0f, 45.0f);
    }

    void Camera::setScreenSize(unsigned int width, unsigned int height) 
    {
        m_screenWidth = width;
        m_screenHeight = height;
    }

    void Camera::setProjectionType(ProjectionType type) 
    {
        m_projectionType = type;
    }

    void Camera::setPosition(const glm::vec3& position) 
    {
        m_position = position;
    }

    void Camera::updateCameraVectors() 
    {
        // Вычисляем новый фронт вектор из углов Эйлера
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);

        // Пересчитываем right и up векторы
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

}