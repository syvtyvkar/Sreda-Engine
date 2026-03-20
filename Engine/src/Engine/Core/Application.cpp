// (c) Nikita Rogalev. All rights reserved.

#include <glad/glad.h>                              // TODO: (УБРАТЬ ОТСЮДА!!!!!!!!!!) Загрузчик OpenGL функций

#include "Application.h"                            // Собственный заголовок класса Application
#include "config.h"                                 // Конфигурационные параметры
#include "Engine/Platform/PlatformUtils.h"          // Утилиты для работы с платформой (пути и т.д.)
#include "Engine/Core/ResourceManager.h"            // Менеджер ресурсов (текстуры, шейдеры и т.п.)
#include "Engine/Core/Log.h"                        // Система логирования
#include "Engine/Platform/IWindow.h"                // Интерфейс окна
#include "Engine/Input/Input.h"                     // Система ввода
#include "Engine/Core/Time.h"                       // Система времени (таймеры, дельта-тайм)

#include <string>
#include <sstream>
#include <vector>

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
        s_Instance->m_currentScene = std::make_unique<Scene>("MainScene");              // Создаём главную сцену с именем "MainScene" и сохраняем её в unique_ptr
        return s_Instance;                                                              // Возвращаем указатель на созданный экземпляр
    }

    void Application::RunApp(std::string InNameApp)                         // Основной метод запуска приложения
    {
        ENGINE_LOG_INFO("Running app...");                                                          // Логируем начало выполнения
        Engine::ResourceManager::getInstance().init(Engine::PlatformUtils::getExecutablePath());    // Инициализация менеджера ресурсов с путём к исполняемому файлу
        Time::TimeSystem::Init();                                                                   // Инициализация системы времени
        AppWindow = WindowAPIFactory::create();                                                     // Создание окна через фабрику (в зависимости от платформы)
        Engine::WindowConfig WindowConfig;                                                          // TODO: ПЕРЕНЕСТИ В КОНФИГИ!!! Настройка параметров окна 
        WindowConfig.wight = 800;
        WindowConfig.height = 600;
        WindowConfig.title = InNameApp;
        WindowConfig.vsync = true;

        if (!AppWindow->Init(WindowConfig))                                                         // Попытка инициализировать окно с заданными параметрами
        {                                                       
            ENGINE_LOG_CRITICAL("Failed to create window!");                                        // Ошибка создания окна
            return;
        }

        m_render = RenderAPIFactory::create();                                                      // Создание рендерера через фабрику (OpenGL, Vulkan и т.д.)
        m_currentScene->SetParentRender(m_render.get());                                            // Устанавливаем для сцены указатель на рендерер (чтобы сцена могла отрисовывать объекты)
        if (!m_render || !m_render.get()->initialize())                                             // Инициализация рендерера
        {
            ENGINE_LOG_ERROR("Failed to initialize renderer!");                                     // Ошибка инициализации
            return;
        }

        m_render->setViewport(0,0,WindowConfig.wight,WindowConfig.height);                          // Установка области вывода (viewport) на весь размер окна
        m_render->setClearColor(Color(0.1f,0.1f,0.1f,1.f));                                         // Установка цвета очистки экрана (тёмно-серый)

        Input::InputSystem::Init(AppWindow.get());                                                  // Инициализация системы ввода с передачей указателя на окно

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                                    // Загрузка OpenGL функций через GLAD (требуется после создания контекста окна)
        {                                  
            ENGINE_LOG_ERROR("Failed to initialize GLAD");                                          // Ошибка инициализации
            return;
        }

        ENGINE_LOG_TRACE("OpenGL Version: {}", *glGetString(GL_VERSION));                           // Вывод информации о версии OpenGL и рабочей директории
        ENGINE_LOG_TRACE("Working Directory: {}", Engine::PlatformUtils::getCurrentWorkingDirectory());

        Engine::CameraConfig camConfig;                                                             // Настройка камеры
        camConfig.position = glm::vec3(0.0f, 0.0f, 3.0f);                                           // Позиция камеры
        camConfig.screenWidth = WindowConfig.wight;                                                 // Ширина экрана для соотношения сторон
        camConfig.screenHeight = WindowConfig.height;                                               // Высота экрана

        // Подписка на событие нажатия клавиши
        Input::InputSystem::GetInstance().OnKeyPressed().Subscribe([&,this](int key, int mods, int repeat) 
        {
            if (Input::InputSystem::InputKeyFromInt(key) != InputKey::X) return;                            // Если нажата не клавиша X, игнорируем
            ENGINE_LOG_INFO("Press exit application...");                                                   // Логируем нажатие
            ExitApp();                                                                                      // Выход из приложения
        });

        m_currentScene->createGameObject("NewObj");                                                         // Создание тестового игрового объекта в сцене

        while (!AppWindow->ShouldClose())                                                                   // ОСНОВНОЙ ИГРОВОЙ ЦИКЛ Пока окно не запросило закрытие
        {
            if (!m_render) return;                                                                          // Если рендерер потерян, выходим (аварийно)
            Time::TimeSystem::Update();                                                                     // Обновление системы времени

            // TODO: Временно, пока система времени в работе...
            float currentFrame = static_cast<float>(glfwGetTime());
            g_deltaTime = currentFrame - g_lastFrame;
            g_lastFrame = currentFrame;
            float dt = Time::TimeSystem::GetDeltaTimeSeconds();
            //////////////////////////////

            m_render->beginFrame();                                                                         // Начало кадра (подготовка рендерера к отрисовке)
            m_render->clear();                                                                              // Очистка буферов цвета и глубины

            m_currentScene->update(g_deltaTime);                                                            // Обновление логики сцены с передачей deltaTime

            m_currentScene->render(m_render.get());                                                         // Отрисовка сцены с использованием текущего рендерера

            m_render->endFrame();                                                                           // Завершение кадра (переключение буферов и т.д.)
            AppWindow->Update();                                                                            // Обработка событий окна (GLFW poll events)
        }
        Input::InputSystem::Shutdown();                                                                     // Выключение системы ввода
        Time::TimeSystem::Shutdown();                                                                       // Выключение системы времени
    }

    Application &Application::Get(){ return *s_Instance;}                                                   // Возвращает ссылку на единственный экземпляр приложения

    void Application::ExitApp()                                                                             // Запрос на выход из приложения (закрытие окна)
    {
        Engine::Application::Get().AppWindow->ExitApp();
    }

    void Application::CloseApp()                                                                            // Закрытие приложения и освобождение всех ресурсов
    {
        AppWindow.reset();          // Уничтожение окна (вызов деструктора)
        m_render.reset();           // Уничтожение рендерера
        m_currentScene.reset();     // Уничтожение текущей сцены
        s_Instance = nullptr;       // Сброс указателя на экземпляр (синглтон)
    }
}