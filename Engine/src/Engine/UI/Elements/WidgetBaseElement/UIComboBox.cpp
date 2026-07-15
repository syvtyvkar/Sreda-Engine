#include "UIComboBox.h"

#include "BaseEngine.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Render/RenderCommand.h"
#include "Engine/UI/Framework/UIRenderCommandList.h"
#include "Engine/Input/Input.h"
#include "Engine/Core/Base/EngineCore.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UIComboBox::UIComboBox()
    {
    }

    void UIComboBox::OnInit()
    {
        SetFocusable(true);
        OnClick().Subscribe(this, &UIComboBox::CallOnClick);
        SetSize(Vector2(100.f,15.f));
    }

    void UIComboBox::OnUpdate(float deltaTime)
    {
    }

    void UIComboBox::OnSelfUICollectCommand(UICommandList &InCmd)
    {
        const Vector2 pos = GetComputedPosition();
        const Vector2 size = GetComputedSize();
        Vector2 quad = Vector2(pos.x + (size.x*0.5f) - ((GetPadding().left + GetPadding().right)/2.f),pos.y + (size.y/2.f));

        InCmd.PushRect({quad,size, TColor::Gray, GetLayout()});

        if (IsHovered())
        {
            InCmd.PushQuad({quad,size, TColor::Gray, GetLayout()});
        }

        Vector2 p0 = (quad + (size*0.5f)) - Vector2(10.f,(size.y/2.f)+3.f);
        Vector2 p1 = (quad + (size*0.5f)) - Vector2(7.5f,(size.y/2.f)-3.f);
        Vector2 p2 = (quad + (size*0.5f)) - Vector2(5.f,(size.y/2.f)+3.f);


        InCmd.PushLine({p0,p1, IsHovered() ? TColor::Black : TColor::Gray, GetLayout()-0.1f});
        InCmd.PushLine({p2,p1, IsHovered() ? TColor::Black : TColor::Gray, GetLayout()-0.1f});
    }

    void UIComboBox::AddOption(std::string InVal)
    {
        Options.push_back(InVal);
    }

    void UIComboBox::RemoveOption(std::string InVal)
    {
        auto it = std::find(Options.begin(), Options.end(), InVal);
        if (it == Options.end()) return;
        Options.erase(it);
    }

    void UIComboBox::CallOnClick()
    {
        Vector2 mousePos = InputSystem::GetMousePosition();
        UIComboBox* self = this;
        std::vector<std::string> LOptions = Options;

        const Vector2 pos = GetComputedPosition();
        const Vector2 size = GetComputedSize();
        Vector2 quad = Vector2(pos.x - ((GetPadding().left + GetPadding().right)/2.f),pos.y + (size.y/2.f)) + Vector2(0.f, size.y);

        EngineCore::GetEngineContext().GetUISystem()->ShowContextMenu(
            quad,
            [self,LOptions](ContextMenuItemBuilder& builder) 
            {
                for (std::string LVal : LOptions)
                {
                    builder.AddItem(LVal, [self](){}, true);
                }
            }
        );
    }
}