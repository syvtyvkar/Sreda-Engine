// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/Render/Shader.h"
#include "Engine/Core/Types.h"
#include <memory>
#include "Engine/Render/Camera.h"

namespace Engine 
{

    class Material 
    {
    public:
        Material();
        ~Material();

        bool loadShader(const std::string& vertexPath, const std::string& fragmentPath);

        void use(const Engine::Camera* InCamera);
        void setModelMatrix(const Matrix4& model);  // Принимает glm (конвертация в Transform)

        Shader* getShader() { return m_shader.get(); }

    private:
        std::unique_ptr<Shader> m_shader;
        Matrix4 m_modelMatrix;
        //Transform m_modelMatrix;
    };

}