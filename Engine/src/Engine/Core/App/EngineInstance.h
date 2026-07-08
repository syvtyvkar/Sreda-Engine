// (c) Nikita Rogalev. All rights reserved.

#pragma once // Preprocessor directive for multiple inclusion guard

#include <string>
#include <vector>

#include "Engine/UI/Framework/UISystem.h"
#include "Engine/Core/Base/EngineCore.h"

namespace Engine
{
    // Unique base instance for a specific application. Essentially, the "heart" of the application for user apps
    class ApplicationInstance
    {
        class Application* m_OwnerApp = nullptr;
    public:
        ApplicationInstance() = default;
        virtual ~ApplicationInstance() {OnEnd();m_OwnerApp = nullptr;};
        virtual void OnInit(class Application* InOwnerApp)
        {
            m_OwnerApp = InOwnerApp; 

            EngineCore::GetEngineDelegates().OnAppUpdFrameDelta().Subscribe(this, &ApplicationInstance::Update);
            EngineCore::GetEngineDelegates().OnAppRender().Subscribe(this, &ApplicationInstance::OnRender);
            EngineCore::GetEngineDelegates().OnAppRenderUI().Subscribe(this, &ApplicationInstance::OnRenderUI);
        }
        virtual void DeInit(){}
        virtual void OnStart(){}
        virtual void OnEnd(){}
        virtual void Update(float DeltaTime)
        {
            EngineCore::GetEngineContext().GetUISystem()->Update(DeltaTime);
        }
        virtual void OnRender(){}
        virtual void OnRenderUI()
        {
            if (EngineCore::GetEngineContext().GetUISystem() != nullptr)
            {
                EngineCore::GetEngineContext().GetUISystem()->Render();
            }
        }

        class Application* GetOwnerApp() {return m_OwnerApp;}
        Engine::UI::UISystem* GetUISystem() {return EngineCore::GetEngineContext().GetUISystem();}

        virtual std::string GetNameApp() {return "Engine application";}
    };
}
