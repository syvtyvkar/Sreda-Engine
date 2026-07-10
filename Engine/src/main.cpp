// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Core/App/Application.h"
//#include "BaseEngine.h"
#include "Engine/Core/Utilities/Types.h"
#include "Engine/Core/Base/EngineCore.h"
#include <string>

int main(int argc, char **argv)
{
    Engine::TRef<Engine::EngineCore> Engine = Engine::CreateRef<Engine::EngineCore>();
    if (!Engine) return -1;
    try
    {
        Engine.get()->PreInit(argc, argv);
        Engine.get()->Init();
        Engine::TRef<Engine::Application> App = Engine::Application::CreateNewApplication();
        App->RunApp(Engine);
        App.reset();
        Engine.get()->Shotdown();
        ENGINE_LOG_INFO("Process close.");
    }
    catch (const std::exception& e)
    {
        Engine::Log::LogSystem::ShowMessageBox(0,"ENGINE CRASH: FATAL ERROR", e.what());
    }

    return 0;
}