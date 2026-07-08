// (c) Nikita Rogalev. All rights reserved.

#include "IUIContext.h"
#include "BaseEngine.h"
#include "Platform/UIAPI/UISreda/UISredaContext.h"
#include <algorithm>

namespace Engine::UI 
{
    TUniquePtr<UIContext> Engine::UI::UIContextFactory::create()
    {

        return CreateUniquePtr<UISredaContext>();

        //return nullptr;
    }
    void UIContext::RegisterWidget(TRef<UIElement> widget)
    {
        if (widget)
            m_widgets.push_back(widget);

        if (GetRootWidget() == nullptr)
        {
            SetRootWidget(widget);
        }
    }
    void UIContext::RemoveWidget(const TRef<UIElement> &widget)
    {
        if (GetRootWidget() == widget)
        {
            m_rootWidget = nullptr;
        }
        auto it = std::find(m_widgets.begin(), m_widgets.end(), widget);
        if (it != m_widgets.end())
            m_widgets.erase(it);
    }
    void UIContext::SetRootWidget(TRef<UIElement> root)
    {
        m_rootWidget = root;
        m_rootWidget.get()->m_ui_context=shared_from_this();
    }
    void UIContext::RenderUIElements(TRef<UIElement> element)
    {
        if (element && element->IsVisible())
            element->OnRender();
    }
}