// (c) Nikita Rogalev. All rights reserved.

#ifdef ENGINE_WINDOW_GLFW
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Render/Render.h"
#include "Engine/Core/Types.h"


namespace Engine 
{

    class OpenGLRenderer : public RenderAPI 
    {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer() override;

        bool initialize() override;
        void shutdown() override;
        virtual void beginFrame() override;   // Подготовка к кадру
        virtual void endFrame() override;     // Present, swap buffers

        void setViewport(int x, int y, int width, int height) override;
        void setClearColor(const Color& color) override;
        void clear() override;

        // Рисование
        void drawMaterial(Material* InMaterial, Transform InTransform) override;
        void drawMesh(Mesh* InMesh, Transform InTransform) override;
        virtual void setupMesh(Mesh* InMesh, Transform InTransform) override;
        virtual void removeMesh(Mesh* InMesh, Transform InTransform) override;

        // Конвертация в типы движка
        static Matrix4 getModelMatrix(Transform InTransform);
        static glm::mat4 toGLM(Matrix4 InMatrix4) {return glm::make_mat4(InMatrix4.data);}
        static glm::mat3 toGLM(Matrix3 InMatrix4) {return glm::make_mat3(InMatrix4.data);}
        static glm::mat2 toGLM(Matrix2 InMatrix4) {return glm::make_mat2(InMatrix4.data);}
        static glm::vec4 toGLM(Vector4 InVector4) { return glm::vec4(InVector4.x, InVector4.y, InVector4.z, InVector4.w); }
        static Vector4 fromGLM(const glm::vec4& v) { return Vector4(v.x, v.y, v.z, v.w); }
        static glm::vec2 toGLM(Vector2 InVector2) { return glm::vec2(InVector2.x, InVector2.y); }
        static Vector2 fromGLM(const glm::vec2& v) { return Vector2(v.x, v.y); }
        static glm::vec3 toGLM(Vector3 InVector3) { return glm::vec3(InVector3.x, InVector3.y, InVector3.z); }
        static Vector3 fromGLM(const glm::vec3& v) { return Vector3(v.x, v.y, v.z); }

        std::string getName() const override { return "OpenGL"; }

    private:
        bool m_initialized = false;
        mutable std::unordered_map<std::string, int> m_uniformLocations;

        // Внутренние методы
        int getUniformLocation(unsigned int m_programID, const std::string& name) const;

        void CheckGLError(const char* location);
    };

}

#endif