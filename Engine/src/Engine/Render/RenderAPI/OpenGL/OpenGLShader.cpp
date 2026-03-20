// (c) Nikita Rogalev. All rights reserved.

#include <glad/glad.h>

#include "OpenGLShader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Engine/Core/Log.h"
#include "Engine/Render/RenderAPI/OpenGL/OpenGLRender.h"

namespace Engine 
{

    OpenGLShader::~OpenGLShader() 
    {
        if (m_programID != 0) 
        {
            glDeleteProgram(m_programID);
        }
    }

    bool OpenGLShader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) 
    {
        ENGINE_LOG_TRACE("Loading vertex shader: {}", vertexPath);
        ENGINE_LOG_TRACE("Loading fragment shader: {}", fragmentPath);
        std::string vertexSource = loadFileToString(vertexPath);
        std::string fragmentSource = loadFileToString(fragmentPath);

        if (vertexSource.empty() || fragmentSource.empty()) 
        {
            std::cerr << "Failed to load shader files!" << std::endl;
            return false;
        }

        return loadFromSource(vertexSource, fragmentSource);
    }

    bool OpenGLShader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) 
    {
        // Компиляция вершинного шейдера
        // ENGINE_LOG_TRACE("Start compile vertex shader: {}", vertexSource);
        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        if (vertexShader == 0) return false;

        // Компиляция фрагментного шейдера
        // ENGINE_LOG_TRACE("Start compile fragment shader: {}", fragmentSource);
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (fragmentShader == 0) 
        {
            glDeleteShader(vertexShader);
            return false;
        }

        // Линковка программы
        if (!linkProgram(vertexShader, fragmentShader)) 
        {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            ENGINE_LOG_WARN("Error link shaders!", fragmentSource);
            return false;
        }

        // Очищаем шейдеры (они уже залинкованы в программу)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        ENGINE_LOG_INFO("Complite shader compile!");

        return true;
    }

    void OpenGLShader::use() const 
    {
        if (m_programID == 0) 
        {
            ENGINE_LOG_ERROR("Trying to use invalid shader program!");
            return;
        }
        glUseProgram(m_programID);
    }

    unsigned int OpenGLShader::compileShader(unsigned int type, const std::string& source) 
    {
        if (glCreateShader == nullptr) 
        {
        ENGINE_LOG_ERROR("ERROR: GLAD not initialized! Call gladLoadGLLoader first!");
        return 0;
        }
        unsigned int shader = glCreateShader(type);

        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        // Проверка ошибок компиляции
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) 
        {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compilation error (" 
                      << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
                      << "):\n" << infoLog << std::endl;
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    bool OpenGLShader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) 
    {
        m_programID = glCreateProgram();
        glAttachShader(m_programID, vertexShader);
        glAttachShader(m_programID, fragmentShader);
        glLinkProgram(m_programID);

        // Проверка ошибок линковки
        int success;
        char infoLog[512];
        glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
        if (!success) 
        {
            glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
            std::cerr << "Shader linking error:\n" << infoLog << std::endl;
            glDeleteProgram(m_programID);
            m_programID = 0;
            return false;
        }

        return true;
    }

    int OpenGLShader::getUniformLocation(const std::string& name) const 
    { 
    if (m_uniformLocations.find(name) != m_uniformLocations.end()) 
    {
        return m_uniformLocations[name];
    }

    int location = glGetUniformLocation(m_programID, name.c_str());
    if (location == -1) 
    {
        std::cerr << "Warning: Uniform '" << name << "' not found!" << std::endl;
    }

    m_uniformLocations[name] = location;
    return location;
}

    std::string OpenGLShader::loadFileToString(const std::string& path) 
    {
        std::ifstream file(path);
        if (!file.is_open()) 
        {
            ENGINE_LOG_WARN("Failed to open shader file: {}", path);
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // === Uniform методы ===

    void OpenGLShader::setBool(const std::string& name, bool value) const 
    {
        glUniform1i(getUniformLocation(name), static_cast<int>(value));
    }

    void OpenGLShader::setInt(const std::string& name, int value) const 
    {
        glUniform1i(getUniformLocation(name), value);
    }

    void OpenGLShader::setFloat(const std::string& name, float value) const 
    {
        glUniform1f(getUniformLocation(name), value);
    }

    void OpenGLShader::setVec2(const std::string& name, const Vector2& value) const 
    {
        glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(OpenGLRenderer::toGLM(value)));
    }

    void OpenGLShader::setVec3(const std::string& name, const Vector3& value) const 
    {
        glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(OpenGLRenderer::toGLM(value)));
    }

    void OpenGLShader::setVec4(const std::string& name, const Vector4& value) const 
    {
        glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(OpenGLRenderer::toGLM(value)));
    }

    void OpenGLShader::setMat2(const std::string& name, const Matrix2& value) const 
    {
        glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(OpenGLRenderer::toGLM(value)));
    }

    void OpenGLShader::setMat3(const std::string& name, const Matrix3& value) const 
    {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(OpenGLRenderer::toGLM(value)));
    }

    void OpenGLShader::setMat4(const std::string& name, const Matrix4& value) const 
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(OpenGLRenderer::toGLM(value)));
    }

}