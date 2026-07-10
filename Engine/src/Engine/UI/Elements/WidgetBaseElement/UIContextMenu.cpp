#include "UIContextMenu.h"
#include "BaseEngine.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/RenderCommand.h"
#include "Engine/Input/Input.h"
#include "Engine/Core/Base/EngineCore.h"
#include "Engine/Platform/IWindow.h"
#include "Engine/UI/Framework/UIBuilder.h"
#include "Engine/UI/Framework/IUIContext.h"

namespace Engine::UI
{
    using namespace Engine::Render;
    using namespace Engine;

    UIContextMenu::UIContextMenu()
    {
        SetVisible(UIVisibleMode::Collapse);
        DepthZ = 99999.f;
    }

    UIContextMenu::~UIContextMenu()
    {
        CloseAll();
    }

    void UIContextMenu::OnInit()
    {
        m_Font = FontManager::GetFontManager().GetFontDefault();
    }

    void UIContextMenu::ShowAt(const Vector2& InPosition, std::function<void(ContextMenuItemBuilder&)> InBuildFunc)
    {
        CloseAll();

        m_Items.clear();
        ContextMenuItemBuilder builder(m_Items);
        InBuildFunc(builder);

        if (m_Items.empty())
            return;

        float menuH = CalculateMenuHeight(m_Items);
        m_MenuPosition = ClampToWindow(InPosition, m_MenuWidth, menuH);
        m_HoveredIndex = -1;
        m_IsOpen = true;
        SetVisible(UIVisibleMode::Visible);

        if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseLeft) || InputSystem::IsMouseButtonJustPressed(InputKey::MouseRight))
        {
            m_InputBlock = true;
        }
    }

    void UIContextMenu::Hide()
    {
        CloseAll();
    }

    void UIContextMenu::CloseAll()
    {
        CloseSubmenu();
        m_IsOpen = false;
        SetVisible(UIVisibleMode::Collapse);
        m_Items.clear();
        m_HoveredIndex = -1;
    }

    void UIContextMenu::OnRender()
    {
        if (!m_IsOpen) return;

        RenderMenu(m_MenuPosition, m_MenuWidth, m_Items, m_HoveredIndex);

        if (m_ActiveSubmenu)
        {
            m_ActiveSubmenu->OnRender();
        }
    }

    void UIContextMenu::RenderMenu(const Vector2& InPos, float InWidth, const std::vector<ContextMenuItem>& InItems, int InHoverIndex)
    {
        if (InItems.empty()) return;

        float menuH = CalculateMenuHeight(InItems);
        Vector2 bgCenter = InPos + Vector2(InWidth * 0.5f, menuH * 0.5f);

        float y = InPos.y + MENU_PADDING;
        int itemIndex = 0;

        for (size_t i = 0; i < InItems.size(); i++)
        {
            const ContextMenuItem& item = InItems[i];

            if (item.Type == ContextMenuItemType::Separator)
            {
                float LSeparatorY = y + 1.0f;
                Vector3 p0 = Vector3( InPos.x + ITEM_PADDING_LEFT, LSeparatorY );
                Vector3 p1 = Vector3( InPos.x + InWidth - ITEM_PADDING_LEFT, LSeparatorY );
                Renderer2D::DrawLine(p0,p1,m_SeparatorColor);
                y += 2.0f;
                continue;
            }

            float itemY = y;
            bool isHovered = (itemIndex == InHoverIndex) && item.Enabled;

            TColor textColor = item.Enabled ? m_TextColor : m_DisabledTextColor;
            std::wstring wlabel(item.Label.begin(), item.Label.end());
            float textX = InPos.x + ITEM_PADDING_LEFT;
            float textY = itemY + DEFAULT_ITEM_HEIGHT * 0.5f + 4.0f;

            if (m_Font)
            {
                Renderer2D::RenderDrawText(wlabel, m_Font->GetAtlasTexture(), m_Font->GetGlyphs(),
                    textX, textY,GetDepthZ(), m_FontSize, textColor);
            }

            if (isHovered)
            {
                Vector2 hoverCenter(InPos.x + InWidth * 0.5f, itemY + DEFAULT_ITEM_HEIGHT * 0.5f);
                Renderer2D::DrawQuad(hoverCenter, Vector2(InWidth - 2.0f, DEFAULT_ITEM_HEIGHT), m_HoverColor);
            }

            if (item.Type == ContextMenuItemType::Submenu && item.Enabled)
            {
                float arrowX = InPos.x + InWidth - ITEM_PADDING_LEFT - SUBMENU_ARROW_SIZE;
                float arrowY = itemY + DEFAULT_ITEM_HEIGHT * 0.5f;

                Renderer2D::DrawQuad(Vector2(arrowX + SUBMENU_ARROW_SIZE * 0.33f, arrowY),Vector2(0.0f, 0.0f), m_SubmenuArrowColor);
                Renderer2D::DrawLine(Vector3(arrowX, arrowY - SUBMENU_ARROW_SIZE * 0.5f),Vector3(arrowX + SUBMENU_ARROW_SIZE, arrowY),m_SubmenuArrowColor);
                Renderer2D::DrawLine(Vector3(arrowX + SUBMENU_ARROW_SIZE, arrowY),Vector3( arrowX, arrowY + SUBMENU_ARROW_SIZE * 0.5f),m_SubmenuArrowColor);
            }

            y += DEFAULT_ITEM_HEIGHT;
            itemIndex++;
        }

        Renderer2D::DrawQuad(bgCenter, Vector2(InWidth, menuH), m_BackgroundColor);
        Renderer2D::DrawRect(Vector3(bgCenter.x, bgCenter.y, 0.0f), Vector2(InWidth, menuH), m_BorderColor,9999);
    }

    void UIContextMenu::OnUpdate(float deltaTime)
    {
        if (!m_IsOpen) return;

        HandleInput();

        if (m_ActiveSubmenu)
        {
            m_ActiveSubmenu->OnUpdate(deltaTime);
        }
    }

    void UIContextMenu::HandleInput()
    {
        if (m_InputBlock)
        {
            if (!InputSystem::IsMouseButtonJustPressed(InputKey::MouseLeft) && !InputSystem::IsMouseButtonJustPressed(InputKey::MouseRight))
            {
                m_InputBlock = false;
            }
            return;
        }
        Vector2 mousePos = InputSystem::GetMousePosition();

        if (m_ActiveSubmenu && m_ActiveSubmenu->IsOpen())
        {
            m_ActiveSubmenu->HandleInput();

            if (m_ActiveSubmenu->IsOpen())
                return;

            CloseSubmenu();
            m_HoveredIndex = -1;
        }

        float menuH = CalculateMenuHeight(m_Items);
        bool overMenu = mousePos.x >= m_MenuPosition.x &&
                        mousePos.x <= m_MenuPosition.x + m_MenuWidth &&
                        mousePos.y >= m_MenuPosition.y &&
                        mousePos.y <= m_MenuPosition.y + menuH;

        if (overMenu)
        {
            m_HoveredIndex = GetHoveredItemIndex(mousePos, m_MenuPosition, m_MenuWidth, m_Items);
        }
        else
        {
            m_HoveredIndex = -1;
        }

        if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseLeft))
        {
            if (overMenu && m_HoveredIndex >= 0 && m_HoveredIndex < (int)m_Items.size())
            {
                const ContextMenuItem& item = m_Items[m_HoveredIndex];

                if (item.Type == ContextMenuItemType::Submenu && item.Enabled)
                {
                    OpenSubmenu(m_HoveredIndex, m_MenuPosition, m_MenuWidth);
                }
                else if (item.Type == ContextMenuItemType::Action && item.Enabled && item.Callback)
                {
                    item.Callback();
                    CloseAll();
                }
            }
            else if (!overMenu)
            {
                CloseAll();
            }
        }
        else if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseRight))
        {
            CloseAll();
        }
    }

    int UIContextMenu::GetHoveredItemIndex(const Vector2& InMousePos, const Vector2& InMenuPos, float InMenuWidth, const std::vector<ContextMenuItem>& InItems) const
    {
        float y = InMenuPos.y + MENU_PADDING;
        int itemIndex = 0;

        for (size_t i = 0; i < InItems.size(); i++)
        {
            const ContextMenuItem& item = InItems[i];

            if (item.Type == ContextMenuItemType::Separator)
            {
                y += 2.0f;
                continue;
            }

            if (InMousePos.y >= y && InMousePos.y <= y + DEFAULT_ITEM_HEIGHT)
            {
                return itemIndex;
            }

            y += DEFAULT_ITEM_HEIGHT;
            itemIndex++;
        }

        return -1;
    }

    void UIContextMenu::OpenSubmenu(int InIndex, const Vector2& InParentPos, float InParentWidth)
    {
        if (InIndex < 0 || InIndex >= (int)m_Items.size()) return;
        if (m_Items[InIndex].Type != ContextMenuItemType::Submenu) return;

        CloseSubmenu();

        float y = InParentPos.y + MENU_PADDING;
        /*for (int i = 0; i < InIndex; i++)
        {
            if (m_Items[i].Type == ContextMenuItemType::Separator)
                y += 2.0f;
            else
                y += DEFAULT_ITEM_HEIGHT;
        }*/

        Vector2 submenuPos(InParentPos.x + InParentWidth, y);

        m_ActiveSubmenu = CreateRef<UIContextMenu>();
        m_ui_context.lock().get()->AddContextWidget(m_ActiveSubmenu);
        m_ActiveSubmenu->OnInit();
        m_ActiveSubmenu->m_Font = m_Font;
        m_ActiveSubmenu->m_FontSize = m_FontSize;
        m_ActiveSubmenu->m_MenuWidth = m_MenuWidth;
        m_ActiveSubmenu->m_BackgroundColor = m_BackgroundColor;
        m_ActiveSubmenu->m_BorderColor = m_BorderColor;
        m_ActiveSubmenu->m_HoverColor = m_HoverColor;
        m_ActiveSubmenu->m_TextColor = m_TextColor;
        m_ActiveSubmenu->m_DisabledTextColor = m_DisabledTextColor;
        m_ActiveSubmenu->m_SeparatorColor = m_SeparatorColor;
        m_ActiveSubmenu->m_SubmenuArrowColor = m_SubmenuArrowColor;

        float submenuH = CalculateMenuHeight(m_Items[InIndex].Children);
        m_ActiveSubmenu->m_MenuPosition = m_ActiveSubmenu->ClampToWindow(submenuPos, m_MenuWidth, submenuH);
        m_ActiveSubmenu->m_Items = m_Items[InIndex].Children;
        m_ActiveSubmenu->m_IsOpen = true;
        m_ActiveSubmenu->SetVisible(UIVisibleMode::Visible);
        m_ActiveSubmenu->m_SubmenuParentIndex = InIndex;
        if (InputSystem::IsMouseButtonJustPressed(InputKey::MouseLeft) || InputSystem::IsMouseButtonJustPressed(InputKey::MouseRight))
        {
            m_ActiveSubmenu->m_InputBlock = true;
        }

        m_SubmenuParentIndex = InIndex;
    }

    void UIContextMenu::CloseSubmenu()
    {
        if (m_ActiveSubmenu)
        {
            m_ActiveSubmenu->CloseSubmenu();
            m_ActiveSubmenu.reset();
            m_SubmenuParentIndex = -1;
        }
    }

    float UIContextMenu::CalculateMenuHeight(const std::vector<ContextMenuItem>& InItems) const
    {
        float height = MENU_PADDING * 2.0f;
        for (const auto& item : InItems)
        {
            if (item.Type == ContextMenuItemType::Separator)
                height += 2.0f;
            else
                height += DEFAULT_ITEM_HEIGHT;
        }
        return height;
    }

    int UIContextMenu::GetItemCount(const std::vector<ContextMenuItem>& InItems) const
    {
        int count = 0;
        for (const auto& item : InItems)
        {
            if (item.Type != ContextMenuItemType::Separator)
                count++;
        }
        return count;
    }

    Vector2 UIContextMenu::ClampToWindow(const Vector2& InPos, float InWidth, float InHeight)
    {
        Vector2 result = InPos;

        ENGINE_ASSERT(GetUIContext().get() != nullptr, "No valid UIContext!");
        IWindow* win = EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindow(GetUIContext().get()->GetWindowContext());
        if (!win) return result;

        int windowW = win->GetWidth();
        int windowH = win->GetHeight();

        if (result.x + InWidth > (float)windowW)
            result.x = (float)windowW - InWidth - 4.0f;
        if (result.x < 0.0f)
            result.x = 4.0f;
        if (result.y + InHeight > (float)windowH)
            result.y = (float)windowH - InHeight - 4.0f;
        if (result.y < 0.0f)
            result.y = 4.0f;

        return result;
    }
}