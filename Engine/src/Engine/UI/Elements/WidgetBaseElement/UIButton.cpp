#include "UIButton.h"

#include "BaseEngine.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/UI/Framework/UIRenderCommandList.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UIButton::UIButton(const std::string &text, const std::string& InFontName)
    {
        m_FontName = InFontName;
        m_text = text;
        m_font = FontManager::GetFontManager().AddFont(InFontName);
        SetText(text);
        SetFocusable(true);
    }

    void UIButton::OnInit()
    {
    }

    Vector2 UIButton::GetComputedSize() const
    {
        if (m_AutoSize && m_font)
        {
            std::wstring wtext(m_text.begin(), m_text.end());
            Vector2 textSize = Font::CalculateTextSize(wtext, m_font.get(), m_FontSize);
            return { textSize.x + (GetPadding().left + GetPadding().right), textSize.y + (GetPadding().bottom + GetPadding().top) };
        }
        return UIWidget::GetComputedSize();
    }

    void UIButton::OnRender()
    {
        if (!IsVisible()) return;

        UIWidget::OnRender();
    }

    void UIButton::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;
        UIWidget::OnUpdate(deltaTime);
    }

    void UIButton::OnSelfUICollectCommand(UICommandList &InCmd)
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();

        float textX = pos.x /*+ size.x * 0.5f*/ /*+ 4.0f*/;
        float textY = pos.y + size.y * 0.5f + 6.0f;

        float quadX = pos.x + (size.x*0.5f) - ((GetPadding().left + GetPadding().right)/2.f);
        float quadY = pos.y + (size.y/2.f);

        InCmd.PushRect({Vector2(quadX, quadY), size, TColor(120,120,120,255), GetLayout()});
        //InCmd.PushText({std::wstring(m_text.begin(), m_text.end()),m_font,Vector2(textX + 1.f, textY + 1.f),GetFontSize(),TColor::Black,-1,GetLayout()});
        InCmd.PushText({std::wstring(m_text.begin(), m_text.end()),m_font,Vector2(textX, textY),GetFontSize(),TextColor,-1,GetLayout()});
        InCmd.PushQuad({Vector2(quadX, quadY), size, IsPressed() ? PressButtonColor : (IsHovered() ? HoverButtonColor : NormalButtonColor), GetLayout()});
    }

    void UIButton::SetFontSize(int size)
    {
        m_FontSize = size;
    }
}