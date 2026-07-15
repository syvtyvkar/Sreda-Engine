#include "EditorMainWidget.h"
#include "Engine/UI/Framework/UIBuilder.h"
#include "Engine/UI/Elements/BaseElement/Element.h"
#include "Engine/Core/App/Application.h"
#include "Engine/UI/Widgets/Editor/UIDockSystem/UIDockExample.h"

#include "Engine/Render/Renderer2D.h"

#include "EditorDemoWidget.h"

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

    SetVerticalAlignment(UIVerticalAlignment::Stretch);
    SetHorizontalAlignment(UIHorizontalAlignment::Stretch);

    m_VerticalBox = UIBuilder::WithProperties<VerticalBox>(UIBuilder::CreateVerticalBox(),[](VerticalBox* widget)
    {

        widget->SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
        widget->SetVerticalAlignment(UIVerticalAlignment::Stretch);
    });

    AddChild(m_VerticalBox);

    m_VerticalBox->AddChild(UIBuilder::WithProperties<EditorMainMenuBar>(CreateRef<EditorMainMenuBar>(),[](EditorMainMenuBar* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
        widget->SetSize(Vector2(widget->GetSize().x,widget->GetSize().y));
    }));

    m_VerticalBox->AddChild(
    UIBuilder::WithProperties<UIDockExample>(CreateRef<UIDockExample>(),[](UIDockExample* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
        widget->SetVerticalAlignment(UIVerticalAlignment::Stretch);
        widget->AddChild(UIBuilder::WithProperties<EditorDemoWidget>(CreateRef<EditorDemoWidget>(),[](EditorDemoWidget* widget)
        {
            widget->SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
            widget->SetVerticalAlignment(UIVerticalAlignment::Stretch);
        }));
    }));
}

void EditorMainWidget::OnRender()
{
    Vector2 pos = GetComputedPosition();
    Vector2 size = GetComputedSize();
    TColor color = TColor::Green;

    UIWidget::OnRender();
}
