// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Core/Application.h"    // Заголовочный файл приложения
#include "Engine/Core/Log.h"            // Система логирования
#include <string>                       // Для работы с строками

/* 
* Точка входа
*/

//extern Engine::EngineLayer* GenerateGame(); // Для подключения кода проектов

int main()
{
    Engine::Log::Init();                                                        // Инициализируем систему логирования
    Engine::Application* App = Engine::Application::CreateNewApplication();     // Создаем приложение
    //App->PushLayer(GenerateGame());                                           // TODO: Временно отключил, мб стоит пересмотреть подход?
    App->RunApp("[OpenGL] Render");                                             // Запуск приложения
    delete App;                                                                 // Закрытие приложения
    ENGINE_LOG_INFO("Process close.");                                          // Оповещаем, что приложение завершено

    return 0;
}