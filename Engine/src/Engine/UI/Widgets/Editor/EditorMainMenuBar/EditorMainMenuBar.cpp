// (c) Nikita Rogalev. All rights reserved.

#include "EditorMainMenuBar.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/RenderCommand.h"

EditorMainMenuBar::EditorMainMenuBar()
{
    
}

void EditorMainMenuBar::OnInit()
{
    UIPadding LPadding = UIPadding(10.f,0.f,10.f,0.f);
    SetPadding(UIPadding(10.f,0.f,0.f,0.f));
    TRef<UIButton> LNewButtonFile = UIBuilder::CreateButton("File");
    LNewButtonFile->SetPadding(LPadding);
    AddChild(LNewButtonFile);
    TRef<UIButton> LNewButtonEdit = UIBuilder::CreateButton("Edit");
    LNewButtonEdit->SetPadding(LPadding);
    AddChild(LNewButtonEdit);
    TRef<UIButton> LNewButtonHelp = UIBuilder::CreateButton("Help");
    LNewButtonHelp->SetPadding(LPadding);
    AddChild(LNewButtonHelp);
    //SetSpacing(5.f);
}

void EditorMainMenuBar::OnRender()
{
    Vector2 pos = GetComputedPosition();
    Vector2 size = GetComputedSize();


    //Renderer2D::DrawQuad(pos+(size*0.5f),size,TColor(50,50,50,255));
    
    __super::OnRender();

    Renderer2D::DrawQuad(pos+(size*0.5f),Vector2(size.x,size.y),TColor::DarkGray);
}
