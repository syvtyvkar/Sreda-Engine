// (c) Nikita Rogalev. All rights reserved.

#include "IUIContext.h"
#include "Engine/Core/Log.h"
#include "Engine/UI/Nuclear/NuclearUIContext.h"

namespace Engine::UI 
{
    TUniquePtr<UIContext> Engine::UI::UIContextFactory::create()
    {

        #ifdef ENGINE_WINDOW_GLFW
        return std::make_unique<NuclearUIContext>();

        //return std::make_unique<SredaUIContext>();
        #endif
        return nullptr;
    }
    void UIContext::RegisterWidget(std::shared_ptr<UIElement> widget)
    {
    }
    void UIContext::RemoveWidget(const std::shared_ptr<UIElement> &widget)
    {
    }
    void UIContext::SetRootWidget(std::shared_ptr<UIElement> root)
    {
    }
    void UIContext::RenderUIElements(std::shared_ptr<UIElement> element)
    {
    }
}