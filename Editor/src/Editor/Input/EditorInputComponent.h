#pragma once

#include <Engine.h>

#include <memory>
#include <functional>
#include <map>

class EditorInputComponent
{
public:
    EditorInputComponent() = default;
    ~EditorInputComponent() = default;

    void Init();
    void DeInit();
    
private:
    Engine::DelegateHandle m_ExitApp;
    Engine::DelegateHandle m_ChangeWindowMode;
    Engine::DelegateHandle m_VisisbleCursor;
    Engine::DelegateHandle m_MoveCamera;
};