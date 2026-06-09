// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Core/Application.h"    // Заголовочный файл приложения
#include "BaseEngine.h"                 // Базовые зависимости движка
#include <string>                       // Для работы с строками
/* 
* Точка входа
*/

int main(int argc, char **argv)
{
    try
    {
    Engine::Log::LogSystem::Init();                                                        // Инициализируем систему логирования
    Engine::Application* App = Engine::Application::CreateNewApplication();     // Создаем приложение
    ENGINE_LOG_INFO("Read arguments program:");
    for (int i = 0; i < argc; ++i) 
    {
        ENGINE_LOG_INFO("- Parameter argument: id: {} arg: {}", i, argv[i]);
    }
    App->RunApp("[OpenGL] Render");                                             // Запуск приложения
    delete App;                                                                 // Закрытие приложения
    ENGINE_LOG_INFO("Process close.");                                          // Оповещаем, что приложение завершено
    }
    catch (const std::exception& e)
    {
        Engine::Log::LogSystem::ShowMessageBox(0,"ENGINE CRASH: FATAL ERROR", e.what());
    }

    return 0;
}