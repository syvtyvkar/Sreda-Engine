#include "UIProgressBar.h"

#include "BaseEngine.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/UI/Framework/UIRenderCommandList.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UIProgressBar::UIProgressBar()
    {
    }

    void UIProgressBar::OnInit()
    {
        SetFocusable(false);
        SetSize(Vector2(100.f,15.f));
    }

    void UIProgressBar::OnUpdate(float deltaTime)
    {
    }

    void UIProgressBar::OnSelfUICollectCommand(UICommandList &InCmd)
    {
        const Vector2 pos = GetComputedPosition();
        const Vector2 size = GetComputedSize();
        Vector2 quad = Vector2(pos.x + (size.x*0.5f) - ((GetPadding().left + GetPadding().right)/2.f),pos.y + (size.y/2.f));
        InCmd.PushRect({quad,size, TColor::Gray, GetLayout()});
        float LPercentCount = size.x/m_PercentMax*m_Percent;
        InCmd.PushQuad({Vector2(quad.x-(LPercentCount*0.5),quad.y),Vector2(LPercentCount,size.y), TColor::Orange, GetLayout()});
    }
}