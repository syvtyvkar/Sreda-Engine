// (c) Nikita Rogalev. All rights reserved.

// Shaders

#pragma once
#include "Engine/Core/Utilities/Types.h"

#include <string>
#include <unordered_map>
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace Engine 
{
    /**
     * @class Shader
     * @brief Абстрактный базовый класс для шейдерной программы.
     *
     * Определяет интерфейс для загрузки, компиляции и использования шейдеров (вершинного и фрагментного).
     * Содержит виртуальные методы для установки uniform-переменных, активации программы и т.д.
     * Конкретные реализации (например, OpenGLShader) должны наследовать этот класс.
     */
    class Shader 
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;

        // Загрузка шейдеров из файлов
        virtual bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {return false;};
        
        // Загрузка из строк (для простых случаев)
        virtual bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {return false;};

        // Активировать шейдерную программу
        virtual void use() const {};

        // Получить ID программы
        virtual unsigned int getID() const { return 0;};

        // === Uniform методы ===
        virtual void setBool(const std::string& name, bool value) const {};
        virtual void setInt(const std::string& name, int value) const {};
        virtual void setFloat(const std::string& name, float value) const {};
        
        virtual void setVec2(const std::string& name, const Vector2& value) const {};
        virtual void setVec3(const std::string& name, const Vector3& value) const {};
        virtual void setVec4(const std::string& name, const Vector4& value) const {};
        
        virtual void setMat2(const std::string& name, const Matrix2& value) const {};
        virtual void setMat3(const std::string& name, const Matrix3& value) const {};
        virtual void setMat4(const std::string& name, const Matrix4& value) const {};

        // Проверка, загружен ли шейдер
        virtual bool isLoaded() const {return false;};

        virtual int getUniformLocation(const std::string& name) const {return 0;};

    protected:

        // Внутренние методы
        virtual unsigned int compileShader(unsigned int type, const std::string& source) {return 0;};
        virtual bool linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {return false;};
        virtual std::string loadFileToString(const std::string& path) {return std::string();};
    };

    class ShaderFactory {
    public:
        static TUniquePtr<Shader> create();
    };

}