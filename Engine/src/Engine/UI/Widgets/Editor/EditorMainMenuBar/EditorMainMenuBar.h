// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <Engine.h>
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Elements/WidgetBaseElement/UIContextMenu.h"

class EditorMainMenuBar : public HorizontalBox
{
public:
    EditorMainMenuBar();
    virtual ~EditorMainMenuBar() = default;

    virtual void OnInit() override;
    virtual void OnRender() override;
};
