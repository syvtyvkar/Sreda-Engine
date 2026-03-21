// (c) Nikita Rogalev. All rights reserved.

#include "Shader.h"
#include "Engine/Render/RenderAPI/OpenGL/OpenGLShader.h"

namespace Engine {

    std::unique_ptr<Shader> ShaderFactory::create() 
    {
        #ifdef ENGINE_WINDOW_GLFW
        return std::make_unique<OpenGLShader>();
        #endif
        return nullptr;
    }

}