// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <Engine.h>
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Widgets/Docking/DockManager.h"
#include "Engine/UI/Widgets/Docking/DockSplitNode.h"
#include "Engine/UI/Widgets/Docking/DockTabHost.h"
#include "Engine/UI/Widgets/Docking/DockWidget.h"

class EditorDockingDemo : public UIWidget
{
public:
    EditorDockingDemo();
    virtual ~EditorDockingDemo() = default;

    void OnInit() override;
    void OnRender() override;
    void OnUpdate(float deltaTime) override;
    
    DockManager& GetDockManager() { return m_dockManager; }

private:
    void CreateDemoPanels();
    TRef<UIWidget> CreatePanelContent(const std::string& label, TColor bgColor);

    DockManager m_dockManager;
    TRef<DockSplitNode> m_rootNode;
    TRef<DockTabHost> m_mainHost;

    TRef<DockWidget> m_viewportWidget;
    TRef<DockWidget> m_contentBrowserWidget;
    TRef<DockWidget> m_detailsWidget;
    TRef<DockWidget> m_outputLogWidget;
};
