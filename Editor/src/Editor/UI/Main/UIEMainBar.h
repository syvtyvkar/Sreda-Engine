#pragma once

#include <Engine.h>
#include <vector>
#include "../Framework/UIEditorBase.h"

class UIEMainBar : public UIEditorBase
{
public:
    UIEMainBar() = default;
    virtual ~UIEMainBar() override = default;

    virtual void OnInit() override;

    virtual void OnRender() override;
    virtual void OnUpdate(float deltaTime) override;

private:
    bool m_NewVerse = false;
    bool m_OpenVerse = false;
    bool m_NewProject = false;
    bool m_OpenProject = false;
    bool m_About = false;
};
