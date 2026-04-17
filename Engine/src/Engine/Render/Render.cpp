// (c) Nikita Rogalev. All rights reserved.

#include "Render.h"
#include "Engine/Render/RenderAPI/OpenGL/OpenGLRender.h"

namespace Engine 
{

    TUniquePtr<RenderAPI> RenderAPIFactory::create() 
    {
        // Здесь можно добавить выбор API через конфиг
        // Например: if (config.api == "DirectX") return std::make_unique<DXRenderer>();
        #ifdef ENGINE_WINDOW_GLFW
        return std::make_unique<OpenGLRenderer>();
        #endif
        return nullptr;
    }

}