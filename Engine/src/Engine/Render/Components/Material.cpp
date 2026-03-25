// (c) Nikita Rogalev. All rights reserved.

#include <glad/glad.h>

#include "Material.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Core/Utilities/Types.h"
#include "Engine/Render/Camera.h"
#include "Engine/Render/RenderAPI/OpenGL/OpenGLRender.h"
#include "Engine/Core/Log.h"

namespace Engine 
{

    Material::Material() : m_shader(ShaderFactory::create()) {}

    Material::~Material() = default;

    bool Material::loadShader(const std::string& vertexPath, const std::string& fragmentPath) 
    {
        return m_shader->loadFromFiles(vertexPath, fragmentPath);
    }

    void Material::use(const Engine::Camera* InCamera) 
    {
        if (m_shader.get() && m_shader.get()->isLoaded()) 
        {
            m_shader.get()->use();
            m_shader.get()->setMat4("model", m_modelMatrix);
            m_shader.get()->setVec3("objectColor", Vector3(1.0f, 0.5f, 0.2f));
            Matrix4 ViewMatrix;
            Matrix4 ProjMatrix;
            std::memcpy(ViewMatrix.data, glm::value_ptr(InCamera->getViewMatrix()), 16 * sizeof(float));
            //std::memcpy(ProjMatrix.data, glm::value_ptr(InCamera->getProjectionMatrix()), 16 * sizeof(float));

            glm::mat4 LGM4 = InCamera->getProjectionMatrix();
            glm::mat4 LView = InCamera->getViewMatrix();
            /*m_shader.get()->setMat4("view", ViewMatrix);
            m_shader.get()->setMat4("projection",  ProjMatrix);*/

            glUniformMatrix4fv(m_shader.get()->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(LView));
            glUniformMatrix4fv(m_shader.get()->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(LGM4));

            //ENGINE_LOG_TRACE("Uniforms set: model, view, projection");
        }
        else
        {
            ENGINE_LOG_ERROR("Shader not loaded!");
        }
    }

    void Material::setModelMatrix(const Matrix4& model) {
        m_modelMatrix = model;
    }

}