// (c) Nikita Rogalev. All rights reserved.

#include <glad/glad.h>                              // TODO: (УБРАТЬ ОТСЮДА!!!!!!!!!!) Загрузчик OpenGL функций

// Window Class
#include "WindowGLFW.h"                         // Заголовок класса
#include <iostream>
#include "Engine/Core/Log.h"                    // Логирование
#include "Engine/Core/Time.h"                   // Система времени (таймеры, дельта-тайм)
#include <string>
#include <sstream>
#include <vector>
#include "Engine/Render/Camera.h"   

namespace Engine
{
    WindowGLFW::WindowGLFW() = default;         // Конструктор по умолчанию
    WindowGLFW::~WindowGLFW()                   // Деструктор: автоматически закрывает окно при уничтожении объекта
    {
        Close();
    }

    /**
     * @brief Инициализирует окно GLFW с заданной конфигурацией.
     * @param config Параметры окна (ширина, высота, заголовок, vsync и т.д.)
     * @return true при успехе, false в противном случае.
     */
    bool WindowGLFW::Init(const WindowConfig& config)
    {
        ENGINE_LOG_INFO("Creating window: {}", config.title);
        if (!glfwInit())                                        // Инициализация библиотеки GLFW
        {
            ENGINE_LOG_ERROR("Failed to initialize GLFW");
        }
        NameWindow=config.title;                                // Сохраняем базовое имя окна (используется в UpdateWindowName)
        // Настройка версии OpenGL (3.3 Core Profile)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Включение MSAA (многократная выборка)
        glfwWindowHint(GLFW_SAMPLES, 4);

        #ifdef __APPLE__
        // Для macOS требуется указать совместимость с форвард-компиляцией
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

         m_window = glfwCreateWindow(config.wight,config.height,config.title.c_str(),nullptr,nullptr);  // Создание окна GLFW

         if (!m_window) {
            ENGINE_LOG_CRITICAL("Failed to create GLFW window");
            glfwTerminate();                                        // Завершаем GLFW, если окно не создано
            return false;
        }

        // Сохраняем размеры окна
        m_width = config.wight;
        m_height = config.height;

        // Делаем контекст OpenGL текущим для этого окна
        glfwMakeContextCurrent(m_window);

        // Включение/выключение вертикальной синхронизации
        glfwSwapInterval(config.vsync ? 1 : 0); //VSync!!!

        // Устанавливаем колбэк изменения размера фреймбуфера
        glfwSetFramebufferSizeCallback(m_window, FramebufferResizeCallback);
        // Сохраняем указатель на объект WindowGLF3 в пользовательских данных GLFW, чтобы иметь доступ к нему в статических колбэках.
        glfwSetWindowUserPointer(m_window, this);

        ENGINE_LOG_INFO("Window ( {} ) has been created successfully", config.title);
        ///////////////////////////////////////////

        m_render = RenderAPIFactory::create();                                                      // Создание рендерера через фабрику (OpenGL, Vulkan и т.д.)
        m_currentScene = std::make_unique<Scene>("MainScene");              // Создаём главную сцену с именем "MainScene" и сохраняем её в unique_ptr
        m_currentScene->SetParentRender(m_render.get());                                            // Устанавливаем для сцены указатель на рендерер (чтобы сцена могла отрисовывать объекты)
        if (!m_render || !m_render.get()->initialize())                                             // Инициализация рендерера
        {
            ENGINE_LOG_ERROR("Failed to initialize renderer!");                                     // Ошибка инициализации
            return false;
        }

        m_render->setViewport(0,0,config.wight,config.height);                          // Установка области вывода (viewport) на весь размер окна
        m_render->setClearColor(Color(0.1f,0.1f,0.1f,1.f));                                         // Установка цвета очистки экрана (тёмно-серый)
        m_currentScene->createGameObject("NewObj");                                                         // Создание тестового игрового объекта в сцене

        return true;
    }

    /**
     * @brief Изменяет заголовок окна, добавляя переданную строку к базовому имени.
     */
    void WindowGLFW::UpdateWindowName(std::string NewName)
    {
        //std::string Empty;
        //glfwSetWindowTitle(m_window,Empty.c_str());
        std::string NewNameResult;
        NewNameResult.append(NameWindow + " ");
        NewNameResult.append(NewName + " FPS");
        glfwSetWindowTitle(m_window,NewNameResult.c_str());
    }

    /**
     * @brief Проверяет, должно ли окно быть закрыто.
     * @return true, если окно запросило закрытие.
     */
    bool WindowGLFW::ShouldClose() const 
    {
        return glfwWindowShouldClose(m_window);
    }

    /**
     * @brief Обновляет окно: обрабатывает события и меняет буферы.
     */
    void WindowGLFW::Update() 
    {
        std::stringstream ss;
        std::string ret;
        ss << Time::TimeSystem::GetFPS();
        ss >> ret;
        UpdateWindowName(ret);

        m_render->beginFrame();                                                                         // Начало кадра (подготовка рендерера к отрисовке)
        m_render->clear();                                                                              // Очистка буферов цвета и глубины

        m_currentScene->update(Time::TimeSystem::GetDeltaTimeSeconds());                                // Обновление логики сцены с передачей deltaTime

        m_currentScene->render(m_render.get());                                                         // Отрисовка сцены с использованием текущего рендерера

        m_render->endFrame();  
        glfwPollEvents();               // Обработка событий (клавиатура, мышь и т.д.)
        glfwSwapBuffers(m_window);      // Переключение переднего и заднего буферов
    }

    /**
     * @brief Устанавливает пользовательскую функцию, вызываемую при изменении размера окна.
     * @param callback Функция, принимающая новую ширину и высоту.
     */
    void WindowGLFW::SetResizeCallback(std::function<void(int, int)> callback) 
    {
        m_resizeCallback = callback;
    }

    /**
     * @brief Закрывает окно и завершает работу GLFW.
     */
    void WindowGLFW::Close() 
    {
        if (m_window) 
        {
            ENGINE_LOG_INFO("The Window close");
            glfwDestroyWindow(m_window);            // Уничтожаем окно GLFW
            m_window = nullptr;
        }
        glfwTerminate();                            // Завершаем GLFW (вызывается даже если окно уже было уничтожено)
    }

    /**
     * @brief Инициирует закрытие окна (устанавливает флаг ShouldClose).
     */
    void WindowGLFW::ExitApp()
    {
        ENGINE_CORE_ASSERT(GetHandle(), "FATAL ERROR: NO VALID APPLICATION!");
        glfwSetWindowShouldClose(GetHandle(), true);
    }

    /**
     * @brief Статический колбэк, вызываемый GLFW при изменении размера фреймбуфера.
     * @param window Указатель на окно GLFW.
     * @param width Новая ширина.
     * @param height Новая высота.
     *
     * Обновляет внутренние размеры, устанавливает viewport OpenGL и вызывает
     * пользовательский колбэк, если он задан.
     */
    void WindowGLFW::FramebufferResizeCallback(GLFWwindow* window, int width, int height) 
    {
        auto* win = reinterpret_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
        if (win) 
        {
            win->m_width = width;
            win->m_height = height;
            
            // Устанавливаем область вывода OpenGL на весь новый размер окна
            glViewport(0, 0, width, height);
            
            // Вызываем пользовательский колбэк, если он зарегистрирован
            if (win->m_resizeCallback) 
            {
                win->m_resizeCallback(width, height);
            }
            win->GetCurrentScene()->GetCamera()->setScreenSize(width,height);
        }
    }
}
//#endif