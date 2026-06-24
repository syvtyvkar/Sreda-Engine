#include "Element.h"

#include "Engine/Core/Log.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UIButton::UIButton(const std::string &text, const std::string& InFontName)
    {
        m_FontName = InFontName;
        m_text = text;
        m_font = FontManager::GetFontManager().AddFont(InFontName);
        SetText(text);
    }

    Vector2 UIButton::GetComputedSize() const
    {
        if (m_AutoSize && m_font)
        {
            std::wstring wtext(m_text.begin(), m_text.end());
            Vector2 textSize = Font::CalculateTextSize(wtext, m_font.get(), m_FontSize);
            return { textSize.x + 2 * m_paddingX, textSize.y + 2 * m_paddingY };
        }
        return UIWidget::GetComputedSize();
    }

    void UIButton::OnRender()
    {
        if (!IsVisible()) return;

        UIWidget::OnRender();

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        if (m_font)
        {
            float textX = pos.x + 4.0f;
            float textY = pos.y + size.y * 0.5f + 6.0f;

            Renderer2D::DrawQuad({pos.x + (size.x/2.f) - (m_paddingX/2.f), pos.y + (size.y/2.f)}, size, IsHovered() ? HoverButtonColor : (IsPressed() ? PressButtonColor : NormalButtonColor));
            Renderer2D::DrawRect({ pos.x + (size.x/2.f) - (m_paddingX/2.f), pos.y + (size.y/2.f), 0.0f }, size, TColor(120,120,120,255));

            Renderer2D::RenderDrawText(
                std::wstring(m_text.begin(), m_text.end()),
                m_font.get()->GetAtlasTexture(),
                m_font.get()->GetGlyphs(),
                textX + 1.f, textY + 1.f, 
                GetFontSize(),
                TColor::Black
            );

            Renderer2D::RenderDrawText(
                std::wstring(m_text.begin(), m_text.end()),
                m_font.get()->GetAtlasTexture(),
                m_font.get()->GetGlyphs(),
                textX, textY, 
                GetFontSize(),
                TextColor
            );
        }
    }

    void UIButton::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;
        UIWidget::OnUpdate(deltaTime);
    }

    void UIButton::SetFontSize(int size)
    {
        m_FontSize = size;
    }

    UITextBlock::UITextBlock(const std::string &InText, const std::string& InFontName)
    {
        m_text = InText;
        m_FontName = InFontName;
        m_font = FontManager::GetFontManager().AddFont(InFontName);
    }

    void UITextBlock::OnRender()
    {
        if (!IsVisible()) return;

        UIWidget::OnRender();

        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        if (m_font)
        {
            float textX = pos.x + m_padding.left;
            float textY = pos.y + m_padding.top + GetFontSize() * 0.85f;

            Renderer2D::RenderDrawText(
                std::wstring(m_text.begin(), m_text.end()),
                m_font.get()->GetAtlasTexture(),
                m_font.get()->GetGlyphs(),
                textX, textY, GetFontSize(), TextColor
            );
        }
    }

    void UITextBlock::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;
        UIWidget::OnUpdate(deltaTime);
    }

    void UITextBlock::SetFontSize(int size)
    {
        m_FontSize = size;
    }
}