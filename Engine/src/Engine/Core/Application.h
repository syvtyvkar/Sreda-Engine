// (c) Nikita Rogalev. All rights reserved.

#pragma once // Директива препроцессора для защиты от множественного включения

#include <string>
#include "Engine/Platform/IWindow.h"        // Интерфейс окна
#include <vector>

namespace Engine
{
    // TODO: Возможно, есть смысл избавиться от этого концепта?
    /*class EngineLayer 
    {
    public:
        EngineLayer() = default;
        virtual ~EngineLayer() = default;
        virtual void OnInit(){}
        virtual void DeInit(){}
        virtual void Update(float DeltaTime){}
        virtual void OnRender(){}
    };*/

    // Главный класс приложения (реализует паттерн Singleton)
    class Application
    {
    public:
        Application();                                          // Конструктор
        ~Application();                                         // Деструктор
        static Application* CreateNewApplication();             // Статический метод для создания единственного экземпляра приложения
        void RunApp(std::string InNameApp);                     // Запуск основного цикла приложения, InNameApp - заголовок окна
        static Application& Get();                              // Получение ссылки на единственный экземпляр (синглтон)
        void ExitApp();                                         // Запрос на выход из приложения (закрытие окна)
    private:
        void CloseApp();                                        // Внутренний метод для освобождения ресурсов при закрытии

        static Application* s_Instance;                         // Указатель на единственный экземпляр (синглтон)
        bool m_running=true;                                    // Флаг работы приложения (управляет циклом)

        std::unique_ptr<Window> AppWindow;                      // Окно приложения
    };
}