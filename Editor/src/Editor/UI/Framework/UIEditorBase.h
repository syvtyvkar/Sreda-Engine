#pragma once

#include <Engine.h>
#include <vector>

class UIEditorBase : public UIElement
{
public:
    UIEditorBase();
    
    virtual ~UIEditorBase() override;

    virtual void OnInit() override;

    virtual void OnRender() override;
    virtual void OnUpdate(float deltaTime) override;
protected:
    bool m_openWindow = true;
};
