#include "EditorMainWidget.h"
#include "Engine/UI/Framework/UIBuilder.h"
#include "Engine/UI/Elements/BaseElement/Element.h"
#include "Engine/Core/App/Application.h"

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

    Application::Get().GetWindow()->OnWindowReSize().Subscribe([&,this](WindowContext cntxt, int x, int y) {
        Vector2 FullSize = Vector2(Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight());
        SetSize(FullSize);
        m_EditorMainMenuBar->SetSize(Vector2(FullSize.x,m_EditorMainMenuBar->GetSize().y));
    });
    Vector2 FullSize = Vector2(Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight());
    SetSize(FullSize);

    SetVerticalAlignment(UIVerticalAlignment::Stretch);
    SetHorizontalAlignment(UIHorizontalAlignment::Stretch);

    m_mainButton = UIBuilder::CreateButton("Test button");
    m_mainButton->OnInit();
    m_mainButton->SetText("Test button 435354");
    //m_mainButton->SetFontSize(48);
    m_mainButton->SetSize({80,30});
    m_mainButton->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    m_mainButton->SetVerticalAlignment(UIVerticalAlignment::Center);

    m_mainButton->OnClick().Subscribe([&]()
    {
        ENGINE_LOG_WARN("CLICK BUTTON! YES!");
    });
    AddChild(m_mainButton);

    TRef<UIImage> LImage = UIBuilder::CreateImage("t_test_texture.png");
    LImage->OnInit();
    LImage->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    LImage->SetVerticalAlignment(UIVerticalAlignment::Center);


    AddChild(LImage);


    m_EditorMainMenuBar = CreateRef<EditorMainMenuBar>();
    m_EditorMainMenuBar->OnInit();
    m_EditorMainMenuBar->SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
    m_EditorMainMenuBar->SetVerticalAlignment(UIVerticalAlignment::Top);
    m_EditorMainMenuBar->SetSize(Vector2(FullSize.x,m_EditorMainMenuBar->GetSize().y));

    AddChild(m_EditorMainMenuBar);

    //TRef<VerticalBox> LVecrticalBox = UIBuilder::CreateVerticalBox();

    TRef<UITextBlock> LText = UIBuilder::CreateTextBlock("Debug mode");
    LText->OnInit();
    LText->SetText("Text block");
    LText->SetSize(45);
    LText->SetHorizontalAlignment(UIHorizontalAlignment::Left);
    LText->SetVerticalAlignment(UIVerticalAlignment::Bottom);
    AddChild(LText);

    TRef<UITextBlock> LTextEditLine = UIBuilder::CreateTextBlock("EditLine Text block:");
    LTextEditLine->OnInit();
    LTextEditLine->SetSize(16);
    LTextEditLine->SetHorizontalAlignment(UIHorizontalAlignment::Left);
    LTextEditLine->SetVerticalAlignment(UIVerticalAlignment::Center);
    LTextEditLine->SetPosition(Vector2(50,-20));
    AddChild(LTextEditLine);

    AddChild(
    UIBuilder::WithProperties<UILineEdit>(UIBuilder::CreateLineEdit(),[](UILineEdit* widget)
    {
        widget->OnInit();
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Center);
        widget->SetSize(Vector2(300.f,20.f));
        widget->SetPosition(Vector2(50,0));  
    }));
}

void EditorMainWidget::OnRender()
{
    Vector2 pos = GetComputedPosition();
    Vector2 size = GetComputedSize();
    TColor color = TColor::Green;

    //Renderer2D::DrawQuad(pos + size * 0.5f, size, color);

    UIWidget::OnRender();
}
