#include "Element.h"

#include "Engine/Core/Log.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UIButton::UIButton(const std::string &text)
    {
        m_text = text;
    }

    void UIButton::OnRender()
    {
        if (!IsVisible()) return;

        UIWidget::OnRender();

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        TRef<Font> font = Font::GetDefault();
        if (font)
        {
            float textX = pos.x + m_style.borderWidth + 4.0f;
            float textY = pos.y + size.y * 0.5f + 6.0f;

            Renderer2D::RenderDrawText(
                std::wstring(m_text.begin(), m_text.end()),
                font->GetAtlasTexture(),
                font->GetGlyphs(),
                textX, textY, 1.0f,
                GetCurrentTextColor()
            );
        }
    }

    void UIButton::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;
        UIWidget::OnUpdate(deltaTime);
    }

    UITextBlock::UITextBlock(const std::string &text)
    {
        m_text = text;
    }

    void UITextBlock::OnRender()
    {
        if (!IsVisible()) return;

        UIWidget::OnRender();

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        TRef<Font> font = Font::GetDefault();
        if (font)
        {
            float textX = pos.x + m_padding.left;
            float textY = pos.y + m_padding.top + m_fontSize * 0.85f;

            Renderer2D::RenderDrawText(
                std::wstring(m_text.begin(), m_text.end()),
                font->GetAtlasTexture(),
                font->GetGlyphs(),
                textX, textY, m_fontSize / 48.0f,
                GetCurrentTextColor()
            );
        }
    }

    void UITextBlock::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;
        UIWidget::OnUpdate(deltaTime);
    }
}