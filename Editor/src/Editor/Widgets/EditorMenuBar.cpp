#include "EditorMenuBar.h"
#include "Engine/UI/Framework/UIBuilder.h"

EditorMenuBar::EditorMenuBar()
{
    m_parent = UIBuilder::CreateHorizontalBox();
    AddChild(UIBuilder::CreateButton("File"));
    AddChild(UIBuilder::CreateButton("Help"));
}

void EditorMenuBar::OnRender()
{
}

void EditorMenuBar::OnUpdate(float deltaTime)
{
    
}
