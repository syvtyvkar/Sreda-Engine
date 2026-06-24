#pragma once

#include "Engine/UI/Framework/UIWidget.h"

namespace Engine::UI
{
    ADD_MULTICAST_DELEGATE_ONE_PARAM(DOnTabAction, int);

    class EditorTabHeader : public UIWidget
    {
    public:
        EditorTabHeader(const std::string& text);
        virtual ~EditorTabHeader() = default;

        void OnRender() override;
        void OnInit() override;

        void SetTabText(const std::string& text) { m_tabText = text; }
        std::string GetTabText() const { return m_tabText; }

        void SetActive(bool active) { m_isActive = active; }
        bool IsActive() const { return m_isActive; }

        void SetTabIndex(int index) { m_tabIndex = index; }
        int GetTabIndex() const { return m_tabIndex; }

        bool IsPointInCloseArea(const Vector2& point) const;

        DOnTabAction& OnTabClose() { return m_onTabClose; }
        DOnTabAction& OnTabSelect() { return m_onTabSelect; }

        static constexpr float TAB_HEIGHT = 32.0f;
        static constexpr float CLOSE_BUTTON_SIZE = 16.0f;

    private:
        void DrawCloseButton(const Vector2& pos, const Vector2& size);

        std::string m_tabText;
        bool m_isActive = false;
        int m_tabIndex = 0;

        DOnTabAction m_onTabClose;
        DOnTabAction m_onTabSelect;
    };
}
