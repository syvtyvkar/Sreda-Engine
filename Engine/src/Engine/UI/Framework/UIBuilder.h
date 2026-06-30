// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <functional>
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Elements/BaseElement/Container.h"
#include "Engine/UI/Elements/BaseElement/Element.h"

#define DEFAULT_FONT "Cuprum"

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

        static TRef<UIButton> CreateButton(const std::string& text = "", const std::string& InFont = DEFAULT_FONT)
        {
            return CreateRef<UIButton>(text, InFont);
        }

        static TRef<UITextBlock> CreateTextBlock(const std::string& InText = "", const std::string& InFont = DEFAULT_FONT)
        {
            return CreateRef<UITextBlock>(InText,InFont);
        }

        static TRef<UIImage> CreateImage(const std::string& InTexture = "")
        {
            return CreateRef<UIImage>(InTexture);
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
