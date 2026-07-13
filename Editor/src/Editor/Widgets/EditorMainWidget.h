#pragma once

#include <Engine.h>
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Widgets/Editor/EditorMainMenuBar/EditorMainMenuBar.h"

class EditorMainWidget : public UIWidget
{
public:
    EditorMainWidget();
    virtual ~EditorMainWidget() = default;

    virtual void OnInit() override;
    virtual void OnRender() override;

    void CallOnWindowReSize(int x, int y); 

private:
    TRef<UIButton> m_mainButton;
    TRef<EditorMainMenuBar> m_EditorMainMenuBar;
};
