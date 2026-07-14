#pragma once

#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/Input/Input.h"

namespace Engine::UI
{
    class UIScrollBox : public UIWidget
    {
    public:
        UIScrollBox();

        void OnRender() override;
        void OnUpdate(float deltaTime) override;
        Vector2 GetComputedSize() const override;

        virtual void OnSelfUICollectCommand(UICommandList& InCmd) override;

        void SetContent(TRef<UIElement> content);
        TRef<UIElement> GetContent() const { return m_content; }

        void SetScrollOffset(const Vector2& offset);
        Vector2 GetScrollOffset() const { return { m_scrollX, m_scrollY }; }

        void SetScrollHorizontal(float scroll);
        void SetScrollVertical(float scroll);
        float GetScrollHorizontal() const { return m_scrollX; }
        float GetScrollVertical() const { return m_scrollY; }

        void SetScrollbarWidth(float width) { m_scrollbarWidth = width; }
        float GetScrollbarWidth() const { return m_scrollbarWidth; }

        void SetShowScrollbars(bool show) { m_showScrollbars = show; }
        bool GetShowScrollbars() const { return m_showScrollbars; }

        void SetScrollSpeed(float speed) { m_scrollSpeed = speed; }
        float GetScrollSpeed() const { return m_scrollSpeed; }

    private:
        void UpdateScrollableSize();
        void ClampScroll();
        void DrawScrollbarVertical(UICommandList& InCmd);
        void DrawScrollbarHorizontal(UICommandList& InCmd);
        bool IsMouseOverScrollbarVertical(const Vector2& mousePos);
        bool IsMouseOverScrollbarHorizontal(const Vector2& mousePos);

        TRef<UIElement> m_content;
        float m_scrollX = 0.0f;
        float m_scrollY = 0.0f;
        float m_scrollbarWidth = 12.0f;
        float m_scrollSpeed = 40.0f;
        bool m_showScrollbars = true;
        bool m_showVertical = false;
        bool m_showHorizontal = false;

        Vector2 m_contentSize = { 0.0f, 0.0f };

        TColor m_scrollbarBgColor = TColor(50, 50, 50, 180);
        TColor m_scrollbarThumbColor = TColor(110, 110, 110, 200);
        TColor m_scrollbarThumbHoverColor = TColor(140, 140, 140, 200);
        bool m_hoveringVertical = false;
        bool m_hoveringHorizontal = false;
        bool m_draggingVertical = false;
        bool m_draggingHorizontal = false;
        float m_dragStartScroll = 0.0f;
        Vector2 m_dragStartMouse;
    };
}
