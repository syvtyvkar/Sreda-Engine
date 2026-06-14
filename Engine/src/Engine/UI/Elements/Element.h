// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/UI/Framework/UIWidget.h"

namespace Engine::UI 
{
    class UIButton : public UIWidget
    {
    public:
        UIButton(const std::string& text);
        void OnRender() override;
        void OnUpdate(float deltaTime) override;
        std::string GetText() const { return m_text; }
        void SetText(const std::string& text) { m_text = text; }

    private:
        std::string m_text;
    };

    class UITextBlock : public UIWidget
    {
    public:
        UITextBlock(const std::string& text);
        void OnRender() override;
        void OnUpdate(float deltaTime) override;

        void SetText(const std::string& text) { m_text = text; }
        std::string GetText() const { return m_text; }

        void SetFontSize(float size) { m_fontSize = size; }
        float GetFontSize() const { return m_fontSize; }

    private:
        std::string m_text;
        float m_fontSize = 16.0f;
    };
}