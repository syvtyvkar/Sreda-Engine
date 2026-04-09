// (c) Nikita Rogalev. All rights reserved.

#include "IUIContext.h"
#include "Engine/Core/Log.h"
//#include "Engine/UI/Rml/RmlUiContext.h"

namespace Engine::UI 
{
    std::unique_ptr<UIContext> Engine::UI::UIContextFactory::create()
    {
        #ifdef ENGINE_WINDOW_GLFW
        //return std::make_unique<RmlUiContext>();
        #endif
        return nullptr;
    }
}