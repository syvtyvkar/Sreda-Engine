// (c) Nikita Rogalev. All rights reserved.

#include "EditorDemoWidget.h"
#include "Engine/UI/Elements/WidgetBaseElement/UICheckBox.h"

EditorDemoWidget::EditorDemoWidget()
{
    SetSize({ 1280, 720 });
    SetHorizontalAlignment(UIHorizontalAlignment::Stretch);
    SetVerticalAlignment(UIVerticalAlignment::Stretch);
    GetStyle().backgroundColor = TColor(30, 30, 30, 255);
    GetStyle().borderWidth = 0.0f;
}

void EditorDemoWidget::OnInit()
{
    __super::OnInit();

    SetVerticalAlignment(UIVerticalAlignment::Stretch);
    SetHorizontalAlignment(UIHorizontalAlignment::Stretch);

    SetPosition(Vector2(GetPosition().x+100.f,GetPosition().y));
    SetSpacing(5.f);

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("Button demo:"),[](UITextBlock* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(UIBuilder::WithProperties<UIButton>(UIBuilder::CreateButton("Test button"),[](UIButton* widget)
    {
        widget->SetText("Test button 435354");
        //widget->SetFontSize(48);
        widget->SetSize({80,30});
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
        widget->OnClick().Subscribe([&]()
        {
            ENGINE_LOG_WARN("CLICK BUTTON! YES!");
        });
    }));

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("Image demo:"),[](UITextBlock* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));


    AddChild(UIBuilder::WithProperties<UIImage>(UIBuilder::CreateImage("t_test_texture.png"),[](UIImage* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("Text demo:"),[](UITextBlock* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("Test TextBlock"),[](UITextBlock* widget)
    {
        widget->SetText("Text block");
        widget->SetSize(45);
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("EditLine demo:"),[](UITextBlock* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(
    UIBuilder::WithProperties<UILineEdit>(UIBuilder::CreateLineEdit(),[](UILineEdit* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
        //widget->SetPosition(Vector2(50,0));  
    }));

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("CheckBox demo:"),[](UITextBlock* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(
    UIBuilder::WithProperties<UICheckBox>(CreateRef<UICheckBox>(),[](UICheckBox* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
        //widget->SetPosition(Vector2(50,0));
    }));

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("ProgressBar demo:"),[](UITextBlock* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(
    UIBuilder::WithProperties<UIProgressBar>(CreateRef<UIProgressBar>(),[](UIProgressBar* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
        //widget->SetPosition(Vector2(50,0));
    }));

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("ComboBox demo:"),[](UITextBlock* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(
    UIBuilder::WithProperties<UIComboBox>(CreateRef<UIComboBox>(),[](UIComboBox* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
        widget->SetOptions({"Variant 1", "Variant 2", "Variant 3"});
        widget->AddOption("Variant ADD");
        //widget->SetPosition(Vector2(50,0));
    }));

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("SliderBox demo:"),[](UITextBlock* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(
    UIBuilder::WithProperties<UISliderBox>(CreateRef<UISliderBox>(),[](UISliderBox* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
        //widget->SetPosition(Vector2(50,0));
    }));

    AddChild(UIBuilder::WithProperties<UITextBlock>(UIBuilder::CreateTextBlock("SpinBox demo:"),[](UITextBlock* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
    }));

    AddChild(
    UIBuilder::WithProperties<UISpinBox>(CreateRef<UISpinBox>(),[](UISpinBox* widget)
    {
        widget->SetHorizontalAlignment(UIHorizontalAlignment::Left);
        widget->SetVerticalAlignment(UIVerticalAlignment::Top);
        //widget->SetPosition(Vector2(50,0));
    }));
}