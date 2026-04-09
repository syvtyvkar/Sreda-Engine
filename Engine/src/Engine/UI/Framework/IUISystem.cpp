// (c) Nikita Rogalev. All rights reserved.

#include "IUISystem.h"
//#include "Engine/UI/Rml/RmlUISystem.h"

namespace Engine::UI 
{

    std::unique_ptr<IUISystem> Engine::UI::UISystemFactory::create()
    {
        #ifdef ENGINE_WINDOW_GLFW
        //return std::make_unique<RmlUISystem>();
        #endif
        return nullptr;
    }

}