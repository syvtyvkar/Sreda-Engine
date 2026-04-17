#include "EditorInputComponent.h"

void EditorInputComponent::Init()
{
    m_ExitApp = Engine::InputSystem::GetInstance().OnKeyPressed().Subscribe([&,this](InputKey key, int mods, int repeat) 
    {
        if (key != InputKey::Escape) return;                                                            // Если нажата не клавиша X, игнорируем
        ENGINE_LOG_INFO("Press exit application...");                                                   // Логируем нажатие
        Application::Get().ExitApp();                                                                   // Выход из приложения
    });

    // Тестим смену режима окна
    m_ChangeWindowMode = Engine::InputSystem::GetInstance().OnKeyPressed().Subscribe([&,this](InputKey key, int mods, int repeat) 
    {
        if (key == InputKey::F11 && Engine::InputSystem::GetInstance().GetKeyState(key) == InputState::Released)
        {
            Window* LWindow = Application::Get().GetWindow();
            if (LWindow)
            {
                LWindow->SetWindowMode((LWindow->GetWindowMode() == WindowMode::Window) ? WindowMode::Borderless :  WindowMode::Window);
            }
        }                                                                                   
    });

    m_VisisbleCursor = Engine::InputSystem::GetInstance().OnKeyPressed().Subscribe([&,this](InputKey key, int mods, int repeat) 
    {
        if (key != InputKey::Control) return;   
        if (Engine::InputSystem::GetInstance().GetKeyState(key) != InputState::Released) return;                                                                                                           
        Engine::InputSystem::GetInstance().SetCursorVisible(!Engine::InputSystem::GetInstance().GetCursorVisible());                                                                                    
    });

    m_MoveCamera = Engine::InputSystem::GetInstance().OnMouseMoved().Subscribe([&,this](float xMove,float yMove)
    {
        if (!Engine::InputSystem::GetInstance().GetCursorVisible() && Application::Get().GetWindow())
        {
            Application::Get().GetWindow()->GetCurrentScene()->GetCamera()->processMouseMovement(xMove,yMove,false);
        }
    });
}

void EditorInputComponent::DeInit()
{
    Engine::InputSystem::GetInstance().OnMouseMoved().Unsubscribe(m_ExitApp);
    Engine::InputSystem::GetInstance().OnMouseMoved().Unsubscribe(m_ChangeWindowMode);
    Engine::InputSystem::GetInstance().OnMouseMoved().Unsubscribe(m_VisisbleCursor);
    Engine::InputSystem::GetInstance().OnMouseMoved().Unsubscribe(m_MoveCamera);
}
