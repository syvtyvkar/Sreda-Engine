// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Core/Application.h"    // Заголовочный файл приложения
#include "Engine/Core/Utilities/Types.h"    // Типы
#include "Engine/Core/Log.h"            // Система логирования
#include <string>                       // Для работы с строками
/* 
* Точка входа
*/

extern Engine::TUniquePtr<Engine::ApplicationInstance> GenerateApplicationInstance(); // Для подключения кода проектов

int main(int argc, char **argv)
{
    Engine::Log::LogSystem::Init();                                                        // Инициализируем систему логирования
    Engine::Application* App = Engine::Application::CreateNewApplication();     // Создаем приложение
    ENGINE_LOG_INFO("Read arguments program:");
    for (int i = 0; i < argc; ++i) 
    {
        ENGINE_LOG_INFO("- Parameter argument: id: {} arg: {}", i, argv[i]);
    }
    App->AddInstance(GenerateApplicationInstance());                              // Экземпляр инстанса приложения
    App->RunApp("[OpenGL] Render");                                             // Запуск приложения
    delete App;                                                                 // Закрытие приложения
    ENGINE_LOG_INFO("Process close.");                                          // Оповещаем, что приложение завершено

    return 0;
}