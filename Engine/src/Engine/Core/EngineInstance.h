// (c) Nikita Rogalev. All rights reserved.

#pragma once // Директива препроцессора для защиты от множественного включения

#include <string>
#include <vector>

namespace Engine
{
    // Уникальный базовый Instance для конкретного приложения. По сути, "сердце" приложения для пользовательских приложений
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

        virtual std::string GetNameApp() {return "Engine application";}

    private:
        class Application* m_OwnerApp = nullptr;
    };
}