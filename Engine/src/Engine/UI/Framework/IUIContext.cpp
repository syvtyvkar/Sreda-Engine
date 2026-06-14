// (c) Nikita Rogalev. All rights reserved.

#include "IUIContext.h"
#include "Engine/Core/Log.h"
#include "Platform/UIAPI/Nuclear/NuclearUIContext.h"
#include <algorithm>

namespace Engine::UI 
{
    TUniquePtr<UIContext> Engine::UI::UIContextFactory::create()
    {

        #ifdef ENGINE_WINDOW_GLFW
        //return std::make_unique<NuclearUIContext>();

        //return std::make_unique<SredaUIContext>();
        #endif
        return nullptr;
    }
    void UIContext::RegisterWidget(std::shared_ptr<UIElement> widget)
    {
        if (widget)
            m_widgets.push_back(widget);
    }
    void UIContext::RemoveWidget(const std::shared_ptr<UIElement> &widget)
    {
        auto it = std::find(m_widgets.begin(), m_widgets.end(), widget);
        if (it != m_widgets.end())
            m_widgets.erase(it);
    }
    void UIContext::SetRootWidget(std::shared_ptr<UIElement> root)
    {
        m_rootWidget = root;
    }
    void UIContext::RenderUIElements(std::shared_ptr<UIElement> element)
    {
        if (element && element->IsVisible())
            element->OnRender();
    }
}