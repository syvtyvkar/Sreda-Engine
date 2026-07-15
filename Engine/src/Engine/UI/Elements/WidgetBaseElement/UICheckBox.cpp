#include "UICheckBox.h"

#include "BaseEngine.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/UI/Framework/UIRenderCommandList.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UICheckBox::UICheckBox()
    {
    }

    void UICheckBox::OnInit()
    {
        SetFocusable(true);
        OnClick().Subscribe(this, &UICheckBox::CallOnClick);
        SetSize(Vector2(15.f,15.f));
    }

    void UICheckBox::OnUpdate(float deltaTime)
    {
    }

    void UICheckBox::OnSelfUICollectCommand(UICommandList &InCmd)
    {
        const Vector2 pos = GetComputedPosition();
        const Vector2 size = GetComputedSize();
        Vector2 quad = Vector2(pos.x + (size.x*0.5f) - ((GetPadding().left + GetPadding().right)/2.f),pos.y + (size.y/2.f));
        InCmd.PushRect({quad,size, TColor::Gray, GetLayout()});
        if (IsHovered())
        {
            InCmd.PushQuad({quad,size, TColor::Gray, GetLayout()});
        }
        if (m_CheckOut)
        {
            InCmd.PushQuad({quad,size, TColor::Orange, GetLayout()});
        }
    }

    void UICheckBox::CallOnClick()
    {
        SetCheckOut(!GetCheckOut());
    }
}