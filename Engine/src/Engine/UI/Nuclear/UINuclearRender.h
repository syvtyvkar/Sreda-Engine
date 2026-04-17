// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include "Engine/UI/Framework/UIRender.h"
#include "Engine/UI/Nuclear/NuclearUIContext.h"

namespace Engine::UI 
{
    class NuclearUIRenderer : public IUIRenderer
    {
    public:
        NuclearUIRenderer(NuclearUIContext* context) : m_context(context) {}
        
        void BeginUI() override;
        void EndUI() override;  
        void RenderButton(const UIButton& Element) override;
        void RenderTextBlock(const UITextBlock& Element) override;
        
    private:
        NuclearUIContext* m_context;
    };
}