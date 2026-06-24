#include "EditorMainWidget.h"
#include "Engine/UI/Framework/UIBuilder.h"
#include "Engine/UI/Elements/Element.h"
#include "Engine/Core/Application.h"

#include "Engine/Render/Renderer2D.h"

EditorMainWidget::EditorMainWidget()
{
    SetSize({ 1280, 720 });
    GetStyle().backgroundColor = TColor(35, 0, 0, 255);
    GetStyle().hoveredBackgroundColor = TColor(35, 0, 0, 255);
    m_style.borderWidth = 0.0f;
}

void EditorMainWidget::OnInit()
{
    __super::OnInit();

    Application::Get().GetWindow()->OnWindowReSize().Subscribe([&,this](int x, int y) {
        SetSize(Vector2(Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight()));
        //SetPosition(Vector2(Application::Get().GetWindow()->GetWidth()/2.f,Application::Get().GetWindow()->GetHeight()/2.f));
    });
    SetSize(Vector2(Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight()));
    SetVerticalAlignment(UIVerticalAlignment::Stretch);
    SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
    //SetPosition(Vector2(Application::Get().GetWindow()->GetWidth()/2.f,Application::Get().GetWindow()->GetHeight()/2.f));


    /*m_mainTabs = CreateRef<EditorTabContainer>();
    m_mainTabs->OnInit();
    m_mainTabs->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    m_mainTabs->SetVerticalAlignment(UIVerticalAlignment::Center);
    AddChild(m_mainTabs);

    auto sceneContent = UIBuilder::CreateTextBlock("Scene View Content");
    sceneContent->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    sceneContent->SetVerticalAlignment(UIVerticalAlignment::Center);
    sceneContent->SetText("Scene View");
    m_mainTabs->AddTab("Scene", sceneContent);*/


    m_mainButton = UIBuilder::CreateButton("Test button", "Cuprum");
    m_mainButton->OnInit();
    m_mainButton->SetText("Test button 435354");
    m_mainButton->SetFontSize(10);
    m_mainButton->SetSize({80,30});
    m_mainButton->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    m_mainButton->SetVerticalAlignment(UIVerticalAlignment::Center);

    AddChild(m_mainButton);

    TRef<UITextBlock> LText = UIBuilder::CreateTextBlock("Text block", "Cuprum");
    LText->OnInit();
    LText->SetText("Text block");
    LText->SetSize(45);
    LText->SetHorizontalAlignment(UIHorizontalAlignment::Right);
    LText->SetVerticalAlignment(UIVerticalAlignment::Bottom);


    AddChild(LText);

    /*auto gameContent = UIBuilder::CreateTextBlock("Game View Content");
    gameContent->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    gameContent->SetVerticalAlignment(UIVerticalAlignment::Center);
    gameContent->SetText("Game View");
    m_mainTabs->AddTab("Game", gameContent);

    auto assetsContent = UIBuilder::CreateTextBlock("Assets Content");
    assetsContent->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    assetsContent->SetVerticalAlignment(UIVerticalAlignment::Center);
    assetsContent->SetText("Assets Browser");
    m_mainTabs->AddTab("Assets", assetsContent);

    auto consoleContent = UIBuilder::CreateTextBlock("Console Content");
    consoleContent->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    consoleContent->SetVerticalAlignment(UIVerticalAlignment::Center);
    consoleContent->SetText("Console");
    m_mainTabs->AddTab("Console", consoleContent);

    auto settingsContent = UIBuilder::CreateTextBlock("Settings Content");
    settingsContent->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    settingsContent->SetVerticalAlignment(UIVerticalAlignment::Center);
    settingsContent->SetText("Settings");
    m_mainTabs->AddTab("Settings", settingsContent, false);*/

    //m_mainTabs->SelectTab(0);
}

void EditorMainWidget::OnRender()
{
    Vector2 pos = GetComputedPosition();
    Vector2 size = GetComputedSize();
    TColor color = TColor::Green;

    //Renderer2D::DrawQuad(pos + size * 0.5f, size, color);

    UIWidget::OnRender();
}
