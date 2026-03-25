// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/Core/Utilities/Types.h"
#include <vector>
#include "Engine/Render/Components/Component.h"
#include "Engine/Render/Components/Material.h"

namespace Engine 
{

    class Mesh : public Component 
    {
        public:
        Mesh() = default;
        ~Mesh()
        {
            m_vertices.clear();
            m_indices.clear();
            m_materialMesh.reset();
        };

        void create(const std::vector<Vertex>& vertices)
        {
            m_vertices = vertices;
            m_indices.clear();
            m_materialMesh = std::make_unique<Material>();
        };
        void create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
        {
            m_vertices = vertices;
            m_indices = indices;
            m_materialMesh = std::make_unique<Material>();
        };

        size_t getVertexCount() const { return m_vertices.size(); }
        size_t getIndexCount() const { return m_indices.size(); }
        bool hasIndices() const { return !m_indices.empty(); }

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::unique_ptr<Material> m_materialMesh;
        
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
        unsigned int m_EBO = 0;

        void SetTransform(Transform InTransformNew) { m_transform = InTransformNew; }
        Transform GetTransform() const { return m_transform; }

    private:
        Transform m_transform;
    };

}