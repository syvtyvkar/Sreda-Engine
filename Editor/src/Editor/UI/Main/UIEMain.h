#pragma once

#include <Engine.h>
#include <vector>
#include "../Framework/UIEditorBase.h"

class UIEMain : public UIEditorBase
{
public:
    UIEMain();
    
    virtual ~UIEMain() override;

    virtual void OnInit() override;

    virtual void OnRender() override;
    virtual void OnUpdate(float deltaTime) override;

};
