// (c) Nikita Rogalev. All rights reserved.

// Shaders

#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Render/Shader.h"

namespace Engine 
{

    class OpenGLShader : public Shader 
    {
    public:
        OpenGLShader() = default;
        ~OpenGLShader();

        // Загрузка шейдеров из файлов
        bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) override;
        
        // Загрузка из строк (для простых случаев)
        bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) override;

        // Активировать шейдерную программу
        void use() const override;

        // Получить ID программы
        unsigned int getID() const override { return m_programID; }

        // === Uniform методы ===
        void setBool(const std::string& name, bool value) const override;
        void setInt(const std::string& name, int value) const override;
        void setFloat(const std::string& name, float value) const override;
        
        void setVec2(const std::string& name, const Vector2& value) const override;
        void setVec3(const std::string& name, const Vector3& value) const override;
        void setVec4(const std::string& name, const Vector4& value) const override;
        
        void setMat2(const std::string& name, const Matrix2& value) const override;
        void setMat3(const std::string& name, const Matrix3& value) const override;
        void setMat4(const std::string& name, const Matrix4& value) const override;

        // Проверка, загружен ли шейдер
        bool isLoaded() const override { return m_programID != 0; }
        int getUniformLocation(const std::string& name) const override;

    private:
        unsigned int m_programID = 0;
        mutable std::unordered_map<std::string, int> m_uniformLocations;

        // Внутренние методы
        unsigned int compileShader(unsigned int type, const std::string& source) override;
        bool linkProgram(unsigned int vertexShader, unsigned int fragmentShader) override;
        
        std::string loadFileToString(const std::string& path) override;
    };

}