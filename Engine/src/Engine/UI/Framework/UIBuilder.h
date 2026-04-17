// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include <functional>
#include "Engine/UI/Elements/Container.h"
#include "Engine/UI/Elements/Element.h"

namespace Engine::UI 
{

    using namespace Engine;
    
    class UIBuilder
    {
    public:
        static std::shared_ptr<VerticalBox> CreateVerticalBox()
        {
            return std::make_shared<VerticalBox>();
        }
        
        static std::shared_ptr<HorizontalBox> CreateHorizontalBox()
        {
            return std::make_shared<HorizontalBox>();
        }
        
        static std::shared_ptr<UIButton> CreateButton(const std::string& text)
        {
            return std::make_shared<UIButton>(text);
        }
        
        static std::shared_ptr<UITextBlock> CreateTextBlock(const std::string& text)
        {
            return std::make_shared<UITextBlock>(text);
        }
        
        template<typename TElement>
        static std::shared_ptr<TElement> WithProperties(
            std::shared_ptr<TElement> element,
            std::function<void(TElement*)> propertiesSetter)
        {
            propertiesSetter(element.get());
            return element;
        }
    };
}

/** @brief Example used
 * 
 * auto myUI = UIBuilder::CreateVerticalBox()
    ->AddChild(UIBuilder::CreateTextBlock("Hello World"))
    ->AddChild(UIBuilder::CreateHorizontalBox()
        ->AddChild(UIBuilder::CreateButton("OK"))
        ->AddChild(UIBuilder::CreateButton("Cancel")));
 * 
 */
