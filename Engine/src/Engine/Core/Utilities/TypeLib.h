// (c) Nikita Rogalev. All rights reserved.

#pragma once // Multiple inclusion guard for header file

#include "Types.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    /*
     Matrix transformations
    */

    inline Matrix4 Translate(const Matrix4& matrix, const Vector3& translation)
    {
        glm::mat4 glmMat = matrix.GetGlmMat4();
        glm::vec3 glmVec = translation.GetGlmVec3();
        glm::mat4 result = glm::translate(glmMat, glmVec);
        return Matrix4(result);
    }

    inline Matrix4 Rotate(const Matrix4& matrix, float angleDegrees, const Vector3& axis)
    {
        glm::mat4 glmMat = matrix.GetGlmMat4();
        glm::vec3 glmAxis = axis.GetGlmVec3();
        glm::mat4 result = glm::rotate(glmMat, glm::radians(angleDegrees), glmAxis);
        return Matrix4(result);
    }

    inline Matrix4 Scale(const Matrix4& matrix, const Vector3& scale)
    {
        glm::mat4 glmMat = matrix.GetGlmMat4();
        glm::vec3 glmScale = scale.GetGlmVec3();
        glm::mat4 result = glm::scale(glmMat, glmScale);
        return Matrix4(result);
    }

    /*
     Projection matrices
    */

    inline Matrix4 Perspective(float fovDegrees, float aspect, float nearPlane, float farPlane)
    {
        glm::mat4 result = glm::perspective(glm::radians(fovDegrees), aspect, nearPlane, farPlane);
        return Matrix4(result);
    }

    inline Matrix4 Ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane)
    {
        glm::mat4 result = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
        return Matrix4(result);
    }

    inline Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
    {
        glm::vec3 glmEye = eye.GetGlmVec3();
        glm::vec3 glmCenter = center.GetGlmVec3();
        glm::vec3 glmUp = up.GetGlmVec3();
        glm::mat4 result = glm::lookAt(glmEye, glmCenter, glmUp);
        return Matrix4(result);
    }

    /*
     Helper functions for matrix operations
    */

    inline Matrix4 Identity()
    {
        return Matrix4::identity();  // already in Types.h
    }

    inline Matrix4 Multiply(const Matrix4& a, const Matrix4& b)
    {
        glm::mat4 glmA = a.GetGlmMat4();
        glm::mat4 glmB = b.GetGlmMat4();
        glm::mat4 result = glmA * glmB;
        return Matrix4(result);
    }

    inline Vector3 Multiply(const Matrix4& matrix, const Vector3& vector)
    {
        glm::mat4 glmMat = matrix.GetGlmMat4();
        glm::vec4 glmVec4(vector.GetGlmVec3(), 1.0f);
        glm::vec4 result = glmMat * glmVec4;
        return Vector3(result.x, result.y, result.z);
    }

    inline Vector4 Multiply(const Matrix4& matrix, const Vector4& vector)
    {
        glm::mat4 glmMat = matrix.GetGlmMat4();
        glm::vec4 glmVec = vector.GetGlmVec4();
        glm::vec4 result = glmMat * glmVec;
        return Vector4(result);
    }

    /*
    Vector operations
    */

    inline float Dot(const Vector3& a, const Vector3& b)
    {
        return glm::dot(a.GetGlmVec3(), b.GetGlmVec3());
    }

    inline Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        glm::vec3 result = glm::cross(a.GetGlmVec3(), b.GetGlmVec3());
        return Vector3(result);
    }

    inline Vector3 Normalize(const Vector3& v)
    {
        glm::vec3 result = glm::normalize(v.GetGlmVec3());
        return Vector3(result);
    }

    inline float Length(const Vector3& v)
    {
        return glm::length(v.GetGlmVec3());
    }

    /* 
    Type conversions 
    */

    inline glm::vec3 ToGlm(const Vector3& v) { return v.GetGlmVec3(); }
    inline glm::mat4 ToGlm(const Matrix4& m) { return m.GetGlmMat4(); }
    inline Vector3 ToVector3(const glm::vec3& v) { return Vector3(v); }
    inline Matrix4 ToMatrix4(const glm::mat4& m) { return Matrix4(m); }
}
