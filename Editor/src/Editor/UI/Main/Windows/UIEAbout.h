#pragma once

#include <Engine.h>
#include <vector>
#include "../../Framework/UIEditorBase.h"

class UIEAbout : public UIEditorBase
{
public:
    UIEAbout() = default;
    virtual ~UIEAbout() override = default;

    virtual void OnInit() override;

    virtual void OnRender() override;
    virtual void OnUpdate(float deltaTime) override;
private:
    TRef<Texture2D> m_LogoImage;
};
