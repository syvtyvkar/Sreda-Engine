#include "UISliderBox.h"

#include "BaseEngine.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/UI/Framework/UIRenderCommandList.h"
#include "Engine/Input/Input.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UISliderBox::UISliderBox()
    {
    }

    void UISliderBox::OnInit()
    {
        SetFocusable(true);
        SetSize(Vector2(100.f,15.f));
        OnHoverBegin().Subscribe(this, &UISliderBox::CallOnHoverBegin);
        OnHoverEnd().Subscribe(this, &UISliderBox::CallOnHoverEnd);
        OnPressBegin().Subscribe(this, &UISliderBox::CallOnPressBegin);
        OnPressEnd().Subscribe(this, &UISliderBox::CallOnPressEnd);
        OnClick().Subscribe(this, &UISliderBox::CallOnClick);

        InputSystem::GetInstance()->OnMouseMoved().Subscribe(this, &UISliderBox::CallOnMoveMouse);
    }

    void UISliderBox::OnUpdate(float deltaTime)
    {
    }

    void UISliderBox::OnSelfUICollectCommand(UICommandList &InCmd)
    {
        const Vector2 pos = GetComputedPosition();
        const Vector2 size = GetComputedSize();
        const Vector2 mousePos = InputSystem::GetMousePosition();
        Vector2 quad = Vector2(pos.x + (size.x*0.5f) - ((GetPadding().left + GetPadding().right)/2.f),pos.y + (size.y/2.f));
        //Background
        InCmd.PushRect({Vector2(quad.x,quad.y),Vector2(size.x, 5.f), TColor::Gray, GetLayout()});

        //Button
        float LPercentCount = size.x/m_MaxVal*m_val;
        Vector2 LButtonPosition = Vector2(pos.x + LPercentCount,quad.y);
        Vector2 LButtonSize = Vector2(10.f, size.y);

        bool LHoverButton = (mousePos.x > LButtonPosition.x) && (pos.x + LButtonSize.x) && (mousePos.y > LButtonPosition.y) && (pos.y + LButtonSize.y);

        InCmd.PushRect({LButtonPosition,LButtonSize, TColor::Gray, GetLayout()});
        InCmd.PushQuad({LButtonPosition,LButtonSize, LHoverButton ? IsPressed() ? TColor::Orange : TColor::Gray : TColor::DarkGray, GetLayout()});
    }

    void UISliderBox::CallOnHoverBegin()
    {
    }
    void UISliderBox::CallOnHoverEnd()
    {
        m_bIsChangeMode = false;
    }
    void UISliderBox::CallOnPressBegin()
    {
        m_bIsChangeMode = true;
    }
    void UISliderBox::CallOnPressEnd()
    {
        m_bIsChangeMode = false;
    }
    void UISliderBox::CallOnMoveMouse(float x, float y)
    {
        if (m_bIsChangeMode)
        {
            if (x == 0.f) return;

            const Vector2 pos = GetComputedPosition();
            const Vector2 size = GetComputedSize();
            const Vector2 mousePos = InputSystem::GetMousePosition();
            Vector2 quad = mousePos - pos;

            float LProcent = size.x*0.01f*quad.x;
            float LOneProcentV = (m_MaxVal-m_MinVal)*0.01f*LProcent;
            SetValue(LOneProcentV);

        }
    }
    void UISliderBox::CallOnClick()
    {
        const Vector2 pos = GetComputedPosition();
        const Vector2 size = GetComputedSize();
        const Vector2 mousePos = InputSystem::GetMousePosition();
        Vector2 quad = mousePos - pos;

        float LProcent = size.x*0.01f*quad.x;
        float LOneProcentV = (m_MaxVal-m_MinVal)*0.01f*LProcent;
        SetValue(LOneProcentV);
    }
}