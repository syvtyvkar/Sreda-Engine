// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Elements/Element.h"
#include "Engine/UI/Elements/Container.h"

namespace Engine::UI 
{

    class IUIRenderer
    {
    public:
        virtual ~IUIRenderer() = default;

        virtual void BeginUI() = 0;
        virtual void EndUI() = 0;

        virtual void RenderWidget(const UIWidget& widget) = 0;
        virtual void RenderButton(const UIButton& button) = 0;
        virtual void RenderTextBlock(const UITextBlock& textBlock) = 0;
        virtual void RenderContainer(const UIContainer& container) = 0;
    };

}