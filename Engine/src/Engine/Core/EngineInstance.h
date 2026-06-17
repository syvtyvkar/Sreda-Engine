// (c) Nikita Rogalev. All rights reserved.

#pragma once // Preprocessor directive for multiple inclusion guard

#include <string>
#include <vector>

namespace Engine
{
    // Unique base instance for a specific application. Essentially, the "heart" of the application for user apps
    class ApplicationInstance
    {
    public:
        ApplicationInstance() = default;
        virtual ~ApplicationInstance() {OnEnd();m_OwnerApp = nullptr;};
        virtual void OnInit(class Application* InOwnerApp){m_OwnerApp = InOwnerApp;}
        virtual void DeInit(){}
        virtual void OnStart(){}
        virtual void OnEnd(){}
        virtual void Update(float DeltaTime){}
        virtual void OnRender(){}
        virtual void OnRenderUI(){}

        class Application* GetOwnerApp() {return m_OwnerApp;}

        virtual std::string GetNameApp() {return "Engine application";}

    private:
        class Application* m_OwnerApp = nullptr;
    };
}
