// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/Render/Camera/Camera.h"
#include "Engine/Render/Shader.h"
#include "Engine/Render/Vertex.h"
#include <vector>
#include <memory>
#include <string>

namespace Engine 
{

    class GameObject 
    {
    public:

        GameObject();
        ~GameObject();

        void Render();
        void Update(float InDeltaTime);

    private:
	    TRef<Shader> m_Shader;
	    TRef<VertexArray> m_VertexArray;
    };
}