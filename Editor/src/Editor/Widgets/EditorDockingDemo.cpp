// (c) Nikita Rogalev. All rights reserved.

#include "EditorDockingDemo.h"
#include "Engine/UI/Framework/UIBuilder.h"
#include "Engine/Core/App/Application.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"

EditorDockingDemo::EditorDockingDemo()
{
    SetSize({ 1280, 720 });
    SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
    SetVerticalAlignment(UIVerticalAlignment::Stretch);
    GetStyle().backgroundColor = TColor(30, 30, 30, 255);
    GetStyle().borderWidth = 0.0f;
}

void EditorDockingDemo::OnInit()
{
    __super::OnInit();

    ENGINE_ASSERT(GetUIContext(), "No valid UIContext!");
    IWindow* LWin = EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindow(
        GetUIContext().get()->GetWindowContext());
    ENGINE_ASSERT(LWin, "Error init EditorDockingDemo. No valid window!");

    m_dockManager.SetMainWindowContext(GetUIContext().get()->GetWindowContext());

    CreateDemoPanels();
}

void EditorDockingDemo::CreateDemoPanels()
{
    m_rootNode = CreateRef<DockSplitNode>(SplitOrientation::Vertical);
    m_rootNode->SetSize(GetSize());
    m_rootNode->SetPosition({ 0, 0 });
    m_rootNode->OnInit();

    m_mainHost = CreateRef<DockTabHost>();
    m_mainHost->OnInit();
    m_mainHost->SetParentSplitNode(m_rootNode);

    m_dockManager.Init(m_rootNode);

    m_viewportWidget = m_dockManager.CreateDockWidget("Viewport",
        CreatePanelContent("VIEWPORT", TColor(30, 40, 50, 255)));

    m_contentBrowserWidget = m_dockManager.CreateDockWidget("Content Browser",
        CreatePanelContent("CONTENT BROWSER", TColor(40, 35, 30, 255)));

    m_detailsWidget = m_dockManager.CreateDockWidget("Details",
        CreatePanelContent("DETAILS", TColor(35, 35, 45, 255)));

    m_outputLogWidget = m_dockManager.CreateDockWidget("Output Log",
        CreatePanelContent("OUTPUT LOG", TColor(35, 45, 35, 255)));

    m_dockManager.AddTabToHost(m_mainHost.get(), m_viewportWidget.get());
    m_dockManager.AddTabToHost(m_mainHost.get(), m_contentBrowserWidget.get());
    m_dockManager.AddTabToHost(m_mainHost.get(), m_detailsWidget.get());
    m_dockManager.AddTabToHost(m_mainHost.get(), m_outputLogWidget.get());

    m_rootNode->AddChildPanel(m_mainHost);

    AddChild(m_rootNode);
}

TRef<UIWidget> EditorDockingDemo::CreatePanelContent(const std::string& label, TColor bgColor)
{
    auto panel = CreateRef<UIWidget>();
    panel->SetSize({ 400, 300 });
    panel->SetPosition({ 0, 0 });
    panel->SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
    panel->SetVerticalAlignment(UIVerticalAlignment::Stretch);
    panel->GetStyle().backgroundColor = bgColor;
    panel->GetStyle().borderWidth = 0.0f;
    panel->OnInit();
    return panel;
}

void EditorDockingDemo::OnRender()
{
    if (!IsVisible()) return;

    Vector2 pos = GetComputedPosition();
    Vector2 size = GetComputedSize();

    //Renderer2D::DrawQuad(pos + size * 0.5f, size, TColor(30, 30, 30, 255));

    UIWidget::OnRender();

    m_dockManager.RenderOverlay();
}

void EditorDockingDemo::OnUpdate(float deltaTime)
{
    if (!IsVisible()) return;

    if (m_rootNode)
    {
        m_rootNode->SetSize(GetComputedSize());
        m_rootNode->SetPosition({ 0, 0 });
    }

    m_dockManager.Update(deltaTime);

    UIWidget::OnUpdate(deltaTime);
}
