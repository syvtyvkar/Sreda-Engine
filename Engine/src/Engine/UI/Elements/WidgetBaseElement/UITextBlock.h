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
    class UITextBlock : public UIWidget
    {
    public:
        UITextBlock(const std::string& InText, const std::string& InFontName);
        void OnRender() override;
        void OnUpdate(float deltaTime) override;

        virtual void OnSelfUICollectCommand(UICommandList& InCmd) override;

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
}