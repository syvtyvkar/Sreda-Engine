#include "UIContextMenu.h"

#include "BaseEngine.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Render/RenderCommand.h"

#include "Engine/UI/Framework/UIBuilder.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UIContextMenu::UIContextMenu()
    {
        
    }

    void UIContextMenu::OnRender()
    {
        if (!IsVisible()) return;

        const Vector2 pos = GetComputedPosition();
        const Vector2 size = GetComputedSize();

        Renderer2D::DrawQuad(pos,size,TColor::White);

        UIWidget::OnRender();
    }
    
    void UIContextMenu::OnUpdate(float deltaTime)
    {

    }

    DOnUIWidgetClick &UIContextMenu::AddButtonContext(std::string InNameButton)
    {
        TRef<UIButton> LNewButton = UIBuilder::CreateButton(InNameButton);
        m_ButtonMap.insert({InNameButton,LNewButton});
        return LNewButton.get()->OnClick();
    }

    void UIContextMenu::RemoveButtonContext(std::string InNameButton)
    {
        if (m_ButtonMap.find(InNameButton) != m_ButtonMap.end())
        {
            m_ButtonMap.erase(InNameButton);
        }
    }

    void UIContextMenu::ClearAllButton()
    {
        for (const auto& [Key,Val] : m_ButtonMap)
        {
            RemoveButtonContext(Key);
        }
        m_ButtonMap.clear();
    }
    bool UIContextMenu::IsVisible() const
    {
        if (m_ButtonMap.size() > 0)
        {
            return UIElement::IsVisible();
        }
        return false;
    }
}