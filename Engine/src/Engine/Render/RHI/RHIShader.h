// (c) Nikita Rogalev. All rights reserved.

#pragma once        // Защита от множественного включения заголовочного файла

#include <string>
#include <glm/glm.hpp>

class IRHIShader
{
public:
    virtual ~IRHIShader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const std::string& name, int value) = 0;
    virtual void SetFloat(const std::string& name, float value) = 0;
    virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
};