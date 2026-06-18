#include "EditorMainWidget.h"
#include "Engine/UI/Framework/UIBuilder.h"

EditorMainWidget::EditorMainWidget()
{
}

void EditorMainWidget::OnInit()
{
    __super::OnInit();

    TRef<HorizontalBox> LHorizontalBox = UIBuilder::CreateHorizontalBox();

    LHorizontalBox.get()->AddChild(UIBuilder::CreateTextBlock("File"));
    LHorizontalBox.get()->AddChild(UIBuilder::CreateTextBlock("Edit"));
    LHorizontalBox.get()->AddChild(UIBuilder::CreateTextBlock("Help"));
    AddChild(LHorizontalBox);
}