
#pragma once

#include <Engine.h>
#include "Engine/UI/Framework/UIWidget.h"


using namespace Engine::UI;


class EditorMainWidget : public UIWidget
{
public:
    EditorMainWidget();
    virtual ~EditorMainWidget() = default;

    virtual void OnInit() override;
};