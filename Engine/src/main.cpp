// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Core/App/Application.h"    // Заголовочный файл приложения
#include "Engine/Core/Base/EngineCore.h"    // Инициализация движка
#include "BaseEngine.h"                     // Базовые зависимости движка
#include <string>                           // Для работы с строками
/* 
* Точка входа
*/

int main(int argc, char **argv)
{
    try
    {
        TRef<Engine::EngineCore> Engine = CreateRef<EngineCore>();
        if (!Engine) return -1;
        Engine.get()->PreInit(argc, argv);
        Engine.get()->Init();
        Engine::Application* App = Engine::Application::CreateNewApplication();     // Создаем приложение
        App->RunApp(Engine);                                             // Запуск приложения
        delete App;                                                                 // Закрытие приложения
        Engine.get()->Shotdown();
        ENGINE_LOG_INFO("Process close.");                                          // Оповещаем, что приложение завершено
    }
    catch (const std::exception& e)
    {
        Engine::Log::LogSystem::ShowMessageBox(0,"ENGINE CRASH: FATAL ERROR", e.what());
    }

    return 0;
}