#pragma once

#include <Engine.h>


using namespace Engine::UI;


class EditorMenuBar : public UIElement
{
public:
    EditorMenuBar();
    virtual ~EditorMenuBar() = default;
        
    virtual void OnRender();
    virtual void OnUpdate(float deltaTime);
};