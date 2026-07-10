// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <Engine.h>
#include "Engine/UI/Framework/UIWidget.h"

class EditorMainMenuBar : public HorizontalBox
{
public:
    EditorMainMenuBar();
    virtual ~EditorMainMenuBar() = default;

    virtual void OnInit() override;
    virtual void OnRender() override;

    void OnCLickFile();
    void OnCLickEdit();
    void OnCLickHelp();

    void OnClickDocumentation();
};
