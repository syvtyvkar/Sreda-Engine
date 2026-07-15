#include "UISpinBox.h"

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

    UISpinBox::UISpinBox()
    {
    }

    void UISpinBox::OnInit()
    {
        SetFocusable(true);
        m_font = FontManager::GetFontManager().AddFont(DEFAULT_FONT_NAME);
        SetSize(Vector2(100.f,15.f));

        OnHoverBegin().Subscribe(this, &UISpinBox::CallOnHoverBegin);
        OnHoverEnd().Subscribe(this, &UISpinBox::CallOnHoverEnd);
        OnPressBegin().Subscribe(this, &UISpinBox::CallOnPressBegin);
        OnPressEnd().Subscribe(this, &UISpinBox::CallOnPressEnd);
        InputSystem::GetInstance()->OnMouseMoved().Subscribe(this, &UISpinBox::CallOnMoveMouse);
    }

    void UISpinBox::OnUpdate(float deltaTime)
    {
    }

    void UISpinBox::OnSelfUICollectCommand(UICommandList &InCmd)
    {
        const Vector2 pos = GetComputedPosition();
        const Vector2 size = GetComputedSize();
        Vector2 quad = Vector2(pos.x + (size.x*0.5f) - ((GetPadding().left + GetPadding().right)/2.f),pos.y + (size.y/2.f));

        Vector2 textCord = Vector2(pos.x, pos.y + size.y * 0.5f + 6.0f);

        InCmd.PushRect({quad,size, TColor::Gray, GetLayout()});
        InCmd.PushQuad({quad,size, IsHovered() ? TColor::Gray : TColor::DarkGray, GetLayout()});

        Vector2 p0 = (quad + (size*0.5f)) - Vector2(10.f,(size.y/2.f)+2.f);
        Vector2 p1 = (quad + (size*0.5f)) - Vector2(7.5f,(size.y/2.f) + 5.f);
        Vector2 p2 = (quad + (size*0.5f)) - Vector2(5.f,(size.y/2.f)+2.f);
        InCmd.PushLine({p0,p1, IsHovered() ? TColor::Black : TColor::Gray, GetLayout()-0.1f});
        InCmd.PushLine({p2,p1, IsHovered() ? TColor::Black : TColor::Gray, GetLayout()-0.1f});

        Vector2 p3 = (quad + (size*0.5f)) - Vector2(10.f,(size.y/2.f) - 2.f);
        Vector2 p4 = (quad + (size*0.5f)) - Vector2(7.5f,(size.y/2.f)- 5.f);
        Vector2 p5 = (quad + (size*0.5f)) - Vector2(5.f,(size.y/2.f) - 2.f);
        InCmd.PushLine({p3,p4, IsHovered() ? TColor::Black : TColor::Gray, GetLayout()-0.1f});
        InCmd.PushLine({p5,p4, IsHovered() ? TColor::Black : TColor::Gray, GetLayout()-0.1f});

        std::string LValText = std::to_string(GetValue());

        InCmd.PushText({std::wstring(LValText.begin(), LValText.end()),m_font,textCord,m_FontSize,TColor::White,-1,GetLayout()});
    }

    void UISpinBox::CallOnHoverBegin()
    {
    }
    void UISpinBox::CallOnHoverEnd()
    {
        m_bIsChangeMode = false;
    }
    void UISpinBox::CallOnPressBegin()
    {
        m_bIsChangeMode = true;
        m_LastMousePos = InputSystem::GetMousePosition();
    }
    void UISpinBox::CallOnPressEnd()
    {
        m_bIsChangeMode = false;
    }
    void UISpinBox::CallOnMoveMouse(float x, float y)
    {
        if (m_bIsChangeMode)
        {
            if (x == 0.f) return;
            if (x > 0.f)
            {
                SetValue(GetValue()+(Engine::Time::TimeSystem::GetDeltaTime()*m_SpeedStep));
            }else
            {
                SetValue(GetValue()-(Engine::Time::TimeSystem::GetDeltaTime()*m_SpeedStep));
            }
            ENGINE_LOG_INFO("SpinBox: {0}", GetValue());
        }
    }
}