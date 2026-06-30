// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/Render/Font/Font.h"

#include "Engine/Render/Vertex.h"
#include "Engine/Render/Shader.h"
#include "Engine/Render/Texture.h"

using namespace Engine::Render;

namespace Engine::UI 
{
    class UIButton : public UIWidget
    {
    public:
        UIButton(const std::string& text, const std::string& InFontName);
        virtual Vector2 GetComputedSize() const override;
        void OnRender() override;
        void OnUpdate(float deltaTime) override;
        std::string GetText() const { return m_text; }
        void SetText(const std::string& InText) { m_text = InText; }
        void SetFontSize(int size);
        int GetFontSize() const { return m_FontSize; }
    private:
        std::string m_text;
        std::string m_FontName;
        TColor TextColor = TColor::White;
        TColor NormalButtonColor= TColor::DarkGray;
        TColor HoverButtonColor= TColor::Gray;
        TColor PressButtonColor = TColor::Orange;
        int m_FontSize = 16.0f;
        TRef<Engine::Render::Font> m_font;
        bool m_AutoSize = true;
    };

    class UITextBlock : public UIWidget
    {
    public:
        UITextBlock(const std::string& InText, const std::string& InFontName);
        void OnRender() override;
        void OnUpdate(float deltaTime) override;

        void SetText(const std::string& text) { m_text = text; }
        std::string GetText() const { return m_text; }

        void SetFontSize(int size);
        int GetFontSize() const { return m_FontSize; }

    private:
        std::string m_text;
        std::string m_FontName;
        TColor TextColor = TColor::White;
        int m_FontSize = 16;
        TRef<Engine::Render::Font> m_font;
    };

    class UIImage : public UIWidget
    {
    public:
        UIImage(const std::string& InImageAsset);
        void OnRender() override;
        void OnUpdate(float deltaTime) override;

    private:
        TRef<Texture2D> m_Texture;
    };

    class UIEditText : public UIWidget
    {
    public:
        UIEditText();
        void OnRender() override;
        void OnUpdate(float deltaTime) override;

    private:
        std::string m_text;
        std::string m_FontName;
        TColor TextColor = TColor::White;
        int m_FontSize = 16;
        TRef<Engine::Render::Font> m_font;
    };
}