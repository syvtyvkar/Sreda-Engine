#pragma once

#include <Engine.h>
#include "Engine/UI/Framework/UIWidget.h"
#include "Engine/UI/Widgets/Editor/EditorTabContainer.h"

class EditorMainWidget : public UIWidget
{
public:
    EditorMainWidget();
    virtual ~EditorMainWidget() = default;

    virtual void OnInit() override;
    virtual void OnRender() override;

private:
    TRef<EditorTabContainer> m_mainTabs;
    TRef<UIButton> m_mainButton;
};
