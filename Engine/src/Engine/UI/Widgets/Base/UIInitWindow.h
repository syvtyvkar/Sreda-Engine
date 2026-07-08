#pragma once

#include <Engine.h>
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Widgets/Editor/EditorTabContainer.h"
#include "Engine/UI/Widgets/Editor/EditorMainMenuBar/EditorMainMenuBar.h"

class UIInitWindow : public UIWidget
{
public:
    UIInitWindow();
    virtual ~UIInitWindow() = default;

    virtual void OnInit() override;
    virtual void OnRender() override;

private:
    TRef<UIImage> m_Background;
};