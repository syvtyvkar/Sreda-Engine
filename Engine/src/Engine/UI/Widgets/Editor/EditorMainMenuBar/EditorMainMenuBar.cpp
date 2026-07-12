// (c) Nikita Rogalev. All rights reserved.

#include "EditorMainMenuBar.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/RenderCommand.h"
#include "Engine/Core/Base/EngineCore.h"

EditorMainMenuBar::EditorMainMenuBar()
{
    
}

void EditorMainMenuBar::OnInit()
{
    UIPadding LPadding = UIPadding(10.f,0.f,10.f,0.f);
    TRef<UIButton> LNewButtonFile = UIBuilder::CreateButton("File");
    LNewButtonFile->SetPadding(LPadding);
    LNewButtonFile->OnClick().Subscribe(this, &EditorMainMenuBar::OnCLickFile);
    AddChild(LNewButtonFile);
    TRef<UIButton> LNewButtonEdit = UIBuilder::CreateButton("Edit");
    LNewButtonEdit->SetPadding(LPadding);
    LNewButtonEdit->OnClick().Subscribe(this, &EditorMainMenuBar::OnCLickEdit);
    AddChild(LNewButtonEdit);
    TRef<UIButton> LNewButtonHelp = UIBuilder::CreateButton("Help");
    LNewButtonHelp->SetPadding(LPadding);
    LNewButtonHelp->OnClick().Subscribe(this, &EditorMainMenuBar::OnCLickHelp);
    AddChild(LNewButtonHelp);
}

void EditorMainMenuBar::OnRender()
{
    Vector2 pos = GetComputedPosition();
    Vector2 size = GetComputedSize();

    __super::OnRender();

    Renderer2D::DrawQuad(pos+Vector2(size.x*0.5f,0.f),Vector2(size.x,size.y+7.f),TColor::DarkGray);
}

void EditorMainMenuBar::OnCLickFile()
{
    EngineCore::GetEngineContext().GetUISystem()->ShowContextMenu(
        InputSystem::GetMousePosition(),
        [&](ContextMenuItemBuilder& builder) {
            builder.AddItem("New", []() { /* TODO: New file */ })
                   .AddItem("Open", []() { /* TODO: Open file */ })
                   .AddItem("Save", []() { /* TODO: Save file */ })
                   //.AddSeparator()
                   .AddSubmenu("Export", [](ContextMenuItemBuilder& sub) {
                       sub.AddItem("As PNG", []() { /* TODO */ })
                          .AddItem("As JPEG", []() { /* TODO */ })
                          .AddItem("As BMP", []() { /* TODO */ });
                   })
                   //.AddSeparator()
                   .AddItem("Exit", [&]() { Application::Get().ExitApp(); });
        }
    );
}

void EditorMainMenuBar::OnCLickEdit()
{
    EngineCore::GetEngineContext().GetUISystem()->ShowContextMenu(
        InputSystem::GetMousePosition(),
        [](ContextMenuItemBuilder& builder) {
            builder.AddItem("Undo", []() { /* TODO */ }, false)
                   .AddItem("Redo", []() { /* TODO */ }, false)
                   .AddSeparator()
                   .AddSubmenu("Preferences", [](ContextMenuItemBuilder& sub) {
                       sub.AddItem("General", []() { /* TODO */ })
                          .AddItem("Rendering", []() { /* TODO */ })
                          .AddItem("Input", []() { /* TODO */ });
                   });
        }
    );
}

void EditorMainMenuBar::OnCLickHelp()
{
    EngineCore::GetEngineContext().GetUISystem()->ShowContextMenu(
        InputSystem::GetMousePosition(),
        [&,this](ContextMenuItemBuilder& builder) {
            builder.AddItem("Documentation", []() { /* TODO */ })
                   .AddItem("About",[&,this]() 
                    { 
                        auto LConfig = Engine::EngineCore::GetEngineContext().GetConfigSystem();                
                        Engine::WindowConfig WindowConfig;                                                          
                        WindowConfig.wight = LConfig->Get<int>("window.width", 800);
                        WindowConfig.height = LConfig->Get<int>("window.height", 600);
                        WindowConfig.title = "About";
                        WindowConfig.vsync = LConfig->Get<bool>("render.vsync",false);
                        WindowConfig.widget = std::make_shared<EditorMainMenuBar>();

                        WindowContext LWinContx = EngineCore::GetEngineContext().GetWindowManager()->CreateEngineWindow(WindowConfig);
                    });
        }
    );
}

void EditorMainMenuBar::OnClickDocumentation()
{
}
