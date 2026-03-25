// (c) Nikita Rogalev. All rights reserved.

#include "Application.h"                            // Собственный заголовок класса Application
#include "Engine/Platform/PlatformUtils.h"          // Утилиты для работы с платформой (пути и т.д.)
#include "Engine/Core/FileSystem/ResourceManager.h"            // Менеджер ресурсов (текстуры, шейдеры и т.п.)
#include "Engine/Core/Log.h"                        // Система логирования
#include "Engine/Platform/IWindow.h"                // Интерфейс окна
#include "Engine/Input/Input.h"                     // Система ввода
#include "Engine/Core/Time.h"                       // Система времени (таймеры, дельта-тайм)
#include "Engine/Core/EngineConfig.h"               // Поддержка конфигурационных файлов

#include "Engine/Render/Camera.h"                       

#include <string>
#include <sstream>
#include <vector>

using namespace EngineConfig;

namespace Engine
{
    Application* Application::s_Instance=nullptr;                           // Статический указатель на единственный экземпляр приложения (синглтон)

    Application::Application() = default;                                   // Конструктор по умолчанию (реализация пустая, т.к. инициализация происходит в CreateNewApplication)
    Application::~Application() {CloseApp();}                               // Деструктор: при уничтожении приложения закрываем его и освобождаем ресурсы

    Application* Application::CreateNewApplication()                        // Статический метод для создания нового экземпляра приложения (синглтон)
    {
        ENGINE_LOG_INFO("Starting application...");                                     // Логируем начало создания
        if (s_Instance)                                                                 // Проверяем, не создан ли уже экземпляр
        {
            ENGINE_LOG_CRITICAL("CRITICAL ERROR! Dublicate application!");              // Ошибка: дубликат
            return nullptr;
        }
        s_Instance = new Application;                                                   // Создаём новый объект
        return s_Instance;                                                              // Возвращаем указатель на созданный экземпляр
    }

    void Application::RunApp(std::string InNameApp)                         // Основной метод запуска приложения
    {
        ENGINE_LOG_INFO("Running app...");                                                          // Логируем начало выполнения
        Engine::ResourceManager::getInstance().init(Engine::PlatformUtils::getExecutablePath());    // Инициализация менеджера ресурсов с путём к исполняемому файлу
        Time::TimeSystem::Init();                                                                   // Инициализация системы времени

        auto& config = EngineConfig::ConfigSystem::Get();                                           // Получаем конфигурационную систему
        config.Init(ENINGE_CONFIG_FILE);                                                            // Инициализация конфигурации движка

        AppWindow = WindowAPIFactory::create();                                                     // Создание окна через фабрику (в зависимости от платформы)
        Engine::WindowConfig WindowConfig;                                                          // TODO: ПЕРЕНЕСТИ В КОНФИГИ!!! Настройка параметров окна 
        WindowConfig.wight = config.Get<int>("window.width", 800);
        WindowConfig.height = config.Get<int>("window.height", 600);
        WindowConfig.title = config.Get<std::string>("window.DefaultWindowName","Engine Render");
        WindowConfig.vsync = config.Get<bool>("render.vsync",false);
        config.Set<int>("window.width", 800);
        config.Set<int>("window.height", 600);
        config.SaveConfig(ENINGE_CONFIG_FILE);

        if (!AppWindow->Init(WindowConfig))                                                         // Попытка инициализировать окно с заданными параметрами
        {                                                       
            ENGINE_LOG_CRITICAL("Failed to create window!");                                        // Ошибка создания окна
            return;
        }

        Engine::InputSystem::Init(AppWindow.get());                                                  // Инициализация системы ввода с передачей указателя на окно

        ENGINE_LOG_TRACE("Working Directory: {}", Engine::PlatformUtils::getCurrentWorkingDirectory());

        // Подписка на событие нажатия клавиши (Выход из демонстрации)
        Engine::InputSystem::GetInstance().OnKeyPressed().Subscribe([&,this](InputKey key, int mods, int repeat) 
        {
            if (key != InputKey::Escape) return;                                                                 // Если нажата не клавиша X, игнорируем
            ENGINE_LOG_INFO("Press exit application...");                                                   // Логируем нажатие
            ExitApp();                                                                                      // Выход из приложения
        });

        Engine::InputSystem::GetInstance().OnKeyPressed().Subscribe([&,this](InputKey key, int mods, int repeat) 
        {
            if (key != InputKey::Control) return;   
            if (Engine::InputSystem::GetInstance().GetKeyState(key) != InputState::Released) return;                                                                                                           
            Engine::InputSystem::GetInstance().SetCursorVisible(!Engine::InputSystem::GetInstance().GetCursorVisible());                                                                                    
        });

        Engine::InputSystem::GetInstance().OnMouseMoved().Subscribe([&,this](float xMove,float yMove)
        {
            if (!Engine::InputSystem::GetInstance().GetCursorVisible() && AppWindow)
            {
                AppWindow->GetCurrentScene()->GetCamera()->processMouseMovement(xMove,yMove,false);
            }
        });

        while (!AppWindow->ShouldClose())                                                                   // ОСНОВНОЙ ИГРОВОЙ ЦИКЛ Пока окно не запросило закрытие
        {
            if (!AppWindow->GetCurrentRender()) return;                                                     // Если рендерер потерян, выходим (аварийно)
            Time::TimeSystem::Update();                                                                     // Обновление системы времени
            AppWindow->Update();                                                                            // Обработка событий окна (GLFW poll events)
        }
        Engine::InputSystem::Shutdown();                                                                     // Выключение системы ввода
        Time::TimeSystem::Shutdown();                                                                       // Выключение системы времени
        config.DeInit();
    }

    Application &Application::Get(){ return *s_Instance;}                                                   // Возвращает ссылку на единственный экземпляр приложения

    void Application::ExitApp()                                                                             // Запрос на выход из приложения (закрытие окна)
    {
        Engine::Application::Get().AppWindow->ExitApp();
    }

    void Application::CloseApp()                                                                            // Закрытие приложения и освобождение всех ресурсов
    {
        AppWindow.reset();          // Уничтожение окна (вызов деструктора)
        s_Instance = nullptr;       // Сброс указателя на экземпляр (синглтон)
    }
}