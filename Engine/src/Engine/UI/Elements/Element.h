// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include "Engine/UI/Framework/UIElement.h"

namespace Engine::UI 
{
    class UIButton : public UIElement
    {
    public:
        UIButton(const std::string& text);
        void OnRender() override;
        void OnUpdate(float deltaTime) override;
        std::string GetText() const {return m_text;}
        
    private:
        std::string m_text;
        bool m_hovered = false;
    };

    class UITextBlock : public UIElement
    {
    public:
        UITextBlock(const std::string& text);
        void OnRender() override;
        void OnUpdate(float deltaTime) override;
        
        void SetText(const std::string& text) {m_text = text;}
        void SetFontSize(float size){m_fontSize = size;}
        void SetColor(const Color& color){m_color = color;}
        std::string GetText() const {return m_text;}
        
    private:
        std::string m_text;
        float m_fontSize = 16.0f;
        Color m_color = {1, 1, 1, 1};
    };
}