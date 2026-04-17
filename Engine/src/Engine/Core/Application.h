// (c) Nikita Rogalev. All rights reserved.

#pragma once // Директива препроцессора для защиты от множественного включения

#include <string>
#include "Engine/Platform/IWindow.h"        // Интерфейс окна
#include "Engine/Core/EngineInstance.h"     // Инстансы
#include <vector>

namespace Engine
{
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
        void AddInstance(TUniquePtr<ApplicationInstance> InInstance);
        Window* GetWindow() const {return m_AppWindow.get();};
    private:
        void CloseApp();                                        // Внутренний метод для освобождения ресурсов при закрытии

        static Application* s_Instance;                         // Указатель на единственный экземпляр (синглтон)
        bool m_running=true;                                    // Флаг работы приложения (управляет циклом)

        TUniquePtr<Window> m_AppWindow;                      // Окно приложения
        TUniquePtr<ApplicationInstance> m_AppInstance;       // Инстанс приложения
    };
}