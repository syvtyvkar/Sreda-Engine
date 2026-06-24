#pragma once

#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Elements/Container.h"
#include "EditorTabHeader.h"
#include <string>
#include <vector>

namespace Engine::UI
{
    struct TabItem
    {
        std::string name;
        TRef<UIElement> content;
        TRef<EditorTabHeader> header;
        bool canClose = true;
    };

    class EditorTabContainer : public UIWidget
    {
    public:
        EditorTabContainer();
        virtual ~EditorTabContainer() = default;

        void OnInit() override;
        void OnRender() override;
        void OnUpdate(float deltaTime) override;

        int AddTab(const std::string& name, TRef<UIElement> content, bool canClose = true);
        void RemoveTab(int index);
        void SelectTab(int index);
        int GetActiveTabIndex() const { return m_activeTabIndex; }
        int GetTabCount() const { return (int)m_tabs.size(); }

        const std::vector<TabItem>& GetTabs() const { return m_tabs; }

        TRef<UIElement> GetContentArea() const { return m_contentArea; }

    private:
        void RebuildTabBar();
        float GetTabStartX() const;

        std::vector<TabItem> m_tabs;
        int m_activeTabIndex = -1;

        TRef<UIWidget> m_tabBarWidget;
        TRef<UIWidget> m_contentArea;

        int m_dragTabIndex = -1;
        bool m_isDragging = false;
        bool m_prevMousePressed = false;
    };
}
