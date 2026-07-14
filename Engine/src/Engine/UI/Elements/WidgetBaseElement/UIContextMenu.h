// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/UI/Framework/UITypes.h"
#include <functional>
#include <string>
#include <vector>

namespace Engine::UI
{
    class UIContextMenu : public UIWidget
    {
        friend class UISystem;
    public:
        UIContextMenu();
        virtual ~UIContextMenu();

        void OnInit();

        void ShowAt(const Vector2& InPosition, std::function<void(ContextMenuItemBuilder&)> InBuildFunc);
        void Hide();

        bool IsOpen() const { return m_IsOpen; }

        void OnRender() override;
        void OnUpdate(float deltaTime) override;

        virtual void OnSelfUICollectCommand(UICommandList& InCmd) override;

        static constexpr float DEFAULT_WIDTH = 140.0f;
        static constexpr float DEFAULT_ITEM_HEIGHT = 16.0f;
        static constexpr float MENU_PADDING = 2.0f;
        static constexpr float ITEM_PADDING_LEFT = 8.0f;
        static constexpr float SUBMENU_ARROW_SIZE = 6.0f;
        static constexpr int DEFAULT_FONT_SIZE = 12;

    private:
        void RenderMenu(const Vector2& InPos, float InWidth, const std::vector<ContextMenuItem>& InItems, int InHoverIndex);
        void HandleInput();
        void CloseAll();
        float CalculateMenuHeight(const std::vector<ContextMenuItem>& InItems) const;
        int GetItemCount(const std::vector<ContextMenuItem>& InItems) const;
        int GetHoveredItemIndex(const Vector2& InMousePos, const Vector2& InMenuPos, float InMenuWidth, const std::vector<ContextMenuItem>& InItems) const;

        void OpenSubmenu(int InIndex, const Vector2& InParentPos, float InParentWidth);
        void CloseSubmenu();

        Vector2 ClampToWindow(const Vector2& InPos, float InWidth, float InHeight);

        std::vector<ContextMenuItem> m_Items;

        bool m_IsOpen = false;
        bool m_InputBlock = false;
        Vector2 m_MenuPosition;
        float m_MenuWidth = DEFAULT_WIDTH;
        int m_HoveredIndex = -1;

        TRef<UIContextMenu> m_ActiveSubmenu = nullptr;
        int m_SubmenuParentIndex = -1;

        TRef<Engine::Render::Font> m_Font;
        int m_FontSize = DEFAULT_FONT_SIZE;

        static constexpr float CORNER_RADIUS = 4.0f;
        static constexpr float BORDER_WIDTH = 1.0f;

        TColor m_BackgroundColor = TColor(40, 40, 43, 245);
        TColor m_BorderColor = TColor(80, 80, 85, 255);
        TColor m_HoverColor = TColor(60, 60, 65, 255);
        TColor m_TextColor = TColor(220, 220, 220, 255);
        TColor m_DisabledTextColor = TColor(100, 100, 100, 255);
        TColor m_SeparatorColor = TColor(70, 70, 75, 255);
        TColor m_SubmenuArrowColor = TColor(160, 160, 160, 255);
    };
}