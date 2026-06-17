// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <functional>
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Elements/Container.h"
#include "Engine/UI/Elements/Element.h"

namespace Engine::UI 
{

    using namespace Engine;

    class UIBuilder
    {
    public:
        static TRef<UIWidget> CreateWidget()
        {
            return CreateRef<UIWidget>();
        }

        static TRef<VerticalBox> CreateVerticalBox()
        {
            return CreateRef<VerticalBox>();
        }

        static TRef<HorizontalBox> CreateHorizontalBox()
        {
            return CreateRef<HorizontalBox>();
        }

        static TRef<UIButton> CreateButton(const std::string& text)
        {
            return CreateRef<UIButton>(text);
        }

        static TRef<UITextBlock> CreateTextBlock(const std::string& text)
        {
            return CreateRef<UITextBlock>(text);
        }

        template<typename TElement>
        static TRef<TElement> WithProperties(
            TRef<TElement> element,
            std::function<void(TElement*)> propertiesSetter)
        {
            propertiesSetter(element.get());
            return element;
        }

        template<typename TElement>
        static TRef<TElement> WithStyle(
            TRef<TElement> element,
            std::function<void(UIWidgetStyle&)> styleSetter)
        {
            if constexpr (std::is_base_of_v<UIWidget, TElement>)
            {
                styleSetter(element->GetStyle());
            }
            return element;
        }
    };
}
