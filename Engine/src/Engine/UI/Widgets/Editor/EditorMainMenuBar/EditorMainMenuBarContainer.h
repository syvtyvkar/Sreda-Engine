// (c) Nikita Rogalev. All rights reserved.

#pragma once

#include <Engine.h>
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Widgets/Editor/EditorTabContainer.h"

class EditorMainMenuBarContainer : public UIWidget
{
public:
    EditorMainMenuBarContainer();
    virtual ~EditorMainMenuBarContainer() = default;

    virtual void OnInit() override;
    virtual void OnRender() override;

private:
    TRef<VerticalBox> m_ContentBox;
};
