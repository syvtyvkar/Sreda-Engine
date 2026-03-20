// (c) Nikita Rogalev. All rights reserved.

#ifdef ENGINE_GLFW

#include <glad/glad.h>


#include "OpenGLRender.h"
#include <iostream>
#include <fstream>
#include <sstream>
//#include "OpenGLRender.h"

#include "Engine/Core/Log.h"

namespace Engine 
{

    OpenGLRenderer::OpenGLRenderer() = default;

    OpenGLRenderer::~OpenGLRenderer() 
    {
        shutdown();
    }

    bool OpenGLRenderer::initialize() 
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        m_initialized = true;
        return true;
    }

    void OpenGLRenderer::shutdown() 
    {
        m_initialized = false;
    }

    void OpenGLRenderer::beginFrame()
    {
        GLint enabled;
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        if (!enabled) {
            ENGINE_LOG_ERROR("Vertex attribute 0 (position) is not enabled!");
        }
    }

    void OpenGLRenderer::endFrame()
    {
    }

    void OpenGLRenderer::setViewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderer::setClearColor(const Color& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderer::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderer::drawMaterial(Material* InMaterial, Transform InTransform) 
    {
        if (!InMaterial) return;

        Matrix4 m_modelMatrix = getModelMatrix(InTransform);

        if (InMaterial->getShader() && InMaterial->getShader()->isLoaded())
        {
            InMaterial->getShader()->use();
            InMaterial->getShader()->setMat4("model", m_modelMatrix);
        }
    }

    void OpenGLRenderer::drawMesh(Mesh *InMesh, Transform InTransform) 
    {
        if (!InMesh || InMesh->m_VAO == 0) 
        {
            ENGINE_LOG_ERROR("drawMesh: Invalid mesh or VAO!");
            return;
        }

        // === Просто привязываем готовый VAO и рисуем ===
        glBindVertexArray(InMesh->m_VAO);
    
        if (!InMesh->m_indices.empty()) 
        {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(InMesh->m_indices.size()), GL_UNSIGNED_INT, 0);
        } 
        else 
        {
            glDrawArrays(GL_TRIANGLES, 0, 
                        static_cast<GLsizei>(InMesh->m_vertices.size()));
        }
    
        // Проверка ошибок
        CheckGLError("glDrawArrays");
    }

    void OpenGLRenderer::setupMesh(Mesh *InMesh, Transform InTransform)
    {
        if (!InMesh)
        {
            ENGINE_LOG_ERROR("NO VALID MESH! SetupMesh skip");
            return;
        }
        // Генерация буферов (только если ещё не созданы)
        if (InMesh->m_VAO == 0) 
        {
            glGenVertexArrays(1, &InMesh->m_VAO);
            glGenBuffers(1, &InMesh->m_VBO);
            if (!InMesh->m_indices.empty()) 
            {
                glGenBuffers(1, &InMesh->m_EBO);
            }
        }

        // === ПРИВЯЗЫВАЕМ VAO ПЕРВЫМ! ===
        glBindVertexArray(InMesh->m_VAO);

        // === Настройка VBO ===
        glBindBuffer(GL_ARRAY_BUFFER, InMesh->m_VBO);
    
        // Конвертируем вершины в формат OpenGL
        struct GLVertex {
           glm::vec3 position;
           glm::vec3 normal;
           glm::vec2 texCoord;
        };
    
        std::vector<GLVertex> glVertices;
        glVertices.reserve(InMesh->m_vertices.size());
        for (const auto& v : InMesh->m_vertices) 
        {
            glVertices.push_back({
            toGLM(v.position), 
            toGLM(v.normal), 
            toGLM(v.texCoord)
        });
        }

        // Загружаем данные в буфер
        glBufferData(GL_ARRAY_BUFFER, glVertices.size() * sizeof(GLVertex),glVertices.data(), GL_STATIC_DRAW);

        // === Настройка EBO (если есть индексы) ===
        if (!InMesh->m_indices.empty()) 
        {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, InMesh->m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,InMesh->m_indices.size() * sizeof(unsigned int),InMesh->m_indices.data(),GL_STATIC_DRAW);
        }

        // === НАСТРОЙКА АТРИБУТОВ (ТОЛЬКО ПОСЛЕ glBindVertexArray!) ===
    
        // Position (location = 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (void*)0);

        // Normal (location = 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (void*)offsetof(GLVertex, normal));

        // TexCoord (location = 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (void*)offsetof(GLVertex, texCoord));

        // === ОТВЯЗЫВАЕМ (опционально, но хорошая практика) ===
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if (!InMesh->m_indices.empty()) 
        {
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    void OpenGLRenderer::removeMesh(Mesh *InMesh, Transform InTransform)
    {
        if (!InMesh)
        {
            ENGINE_LOG_ERROR("NO VALID MESH! SetupMesh skip");
            return;
        }

        if (InMesh->m_VAO != 0) glDeleteVertexArrays(1, &InMesh->m_VAO);
        if (InMesh->m_VBO != 0) glDeleteBuffers(1, &InMesh->m_VBO);
        if (InMesh->m_EBO != 0) glDeleteBuffers(1, &InMesh->m_EBO);
    }

    Matrix4 OpenGLRenderer::getModelMatrix(Transform InTransform)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, toGLM(InTransform.position));
        model = glm::rotate(model, glm::radians(InTransform.rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(InTransform.rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(InTransform.rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, toGLM(InTransform.scale));

        Matrix4 LResult;
        std::memcpy(LResult.data, glm::value_ptr(model), 16 * sizeof(float));
        return LResult;
    }

    int OpenGLRenderer::getUniformLocation(unsigned int m_programID, const std::string &name) const
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
    
    void OpenGLRenderer::CheckGLError(const char *location)
    {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) 
        {
        ENGINE_LOG_ERROR("OpenGL error at {}: {}", location, err);
        }
    }
}

#endif