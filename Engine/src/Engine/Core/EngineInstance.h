// (c) Nikita Rogalev. All rights reserved.

#pragma once // Preprocessor directive for multiple inclusion guard

#include <string>
#include <vector>

#include "Engine/UI/Framework/UISystem.h"

namespace Engine
{
    // Unique base instance for a specific application. Essentially, the "heart" of the application for user apps
    class ApplicationInstance
    {
        class Application* m_OwnerApp = nullptr;
        TUniquePtr<Engine::UI::UISystem> m_UISystem = nullptr;
    public:
        ApplicationInstance() = default;
        virtual ~ApplicationInstance() {OnEnd();m_OwnerApp = nullptr; m_UISystem.reset();};
        virtual void OnInit(class Application* InOwnerApp)
        {
            m_OwnerApp = InOwnerApp; 
            m_UISystem = CreateUniquePtr<Engine::UI::UISystem>();
            m_UISystem.get()->Initialize();
        }
        virtual void DeInit(){}
        virtual void OnStart(){}
        virtual void OnEnd(){}
        virtual void Update(float DeltaTime)
        {
            m_UISystem.get()->Update(DeltaTime);
        }
        virtual void OnRender(){}
        virtual void OnRenderUI()
        {
            if (m_UISystem.get() != nullptr)
            {
                m_UISystem.get()->Render();
            }
        }

        class Application* GetOwnerApp() {return m_OwnerApp;}
        Engine::UI::UISystem* GetUISystem() {return m_UISystem.get();}

        virtual std::string GetNameApp() {return "Engine application";}
    };
}
