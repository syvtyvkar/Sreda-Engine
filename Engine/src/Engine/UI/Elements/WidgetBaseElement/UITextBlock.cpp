#include "UITextBlock.h"

#include "BaseEngine.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/UI/Framework/UIRenderCommandList.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UITextBlock::UITextBlock(const std::string &InText, const std::string& InFontName)
    {
        m_text = InText;
        m_FontName = InFontName;
        m_font = FontManager::GetFontManager().AddFont(InFontName);
    }

    void UITextBlock::OnInit()
    {
    }

    void UITextBlock::OnRender()
    {
        if (!IsVisible()) return;

        UIWidget::OnRender();
    }

    void UITextBlock::OnUpdate(float deltaTime)
    {
        if (!IsVisible()) return;
        UIWidget::OnUpdate(deltaTime);
    }

    void UITextBlock::OnSelfUICollectCommand(UICommandList& InCmd)
    {
        Vector2 pos = GetComputedPosition();
        Vector2 size = GetComputedSize();
        float textX = pos.x + m_padding.left;
        float textY = pos.y + m_padding.top + GetFontSize() * 0.85f;

        InCmd.PushText({std::wstring(m_text.begin(), m_text.end()),m_font,Vector2(textX, textY),GetFontSize(),TextColor,-1,GetLayout()});
    }

    void UITextBlock::SetFontSize(int size)
    {
        m_FontSize = size;
    }
}