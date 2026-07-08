#include "UIInitWindow.h"

#include "Engine/UI/Framework/UIBuilder.h"
#include "Engine/UI/Elements/BaseElement/Element.h"
#include "Engine/Core/App/Application.h"

#include "Engine/Render/Renderer2D.h"

UIInitWindow::UIInitWindow()
{
    SetSize({ 500, 500 });
    GetStyle().backgroundColor = TColor(35, 0, 0, 255);
    GetStyle().hoveredBackgroundColor = TColor(35, 0, 0, 255);
    m_style.borderWidth = 0.0f;
}

void UIInitWindow::OnInit()
{
    __super::OnInit();

    /*ENGINE_ASSERT(GetUIContext(), "No valid UIContext!");
    IWindow* LWin = EngineCore::GetEngineContext().GetWindowManager()->GetEngineWindow(GetUIContext().get()->GetWindowContext());
    ENGINE_ASSERT(LWin, "Error init EditorMainWidget. No valid window!");*/

    /*Vector2 FullSize = Vector2(LWin->GetWidth(), ALWin->GetHeight());
    SetSize(FullSize);

    SetVerticalAlignment(UIVerticalAlignment::Stretch);
    SetHorizontalAlignment(UIHorizontalAlignment::Stretch);

    TRef<UIImage> LImage = UIBuilder::CreateImage("t_test_texture.png");
    LImage->OnInit();
    LImage->SetHorizontalAlignment(UIHorizontalAlignment::Center);
    LImage->SetVerticalAlignment(UIVerticalAlignment::Center);
    AddChild(LImage);*/
}

void UIInitWindow::OnRender()
{
    Vector2 pos = GetComputedPosition();
    Vector2 size = GetComputedSize();
    TColor color = TColor::DarkGray;

    UIWidget::OnRender();
}