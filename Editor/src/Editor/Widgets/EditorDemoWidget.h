// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <Engine.h>
#include "Engine/UI/Framework/UIWidget.h"

class EditorDemoWidget : public VerticalBox
{
public:
    EditorDemoWidget();
    virtual ~EditorDemoWidget() = default;

    void OnInit() override;
};
