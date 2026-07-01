// (c) Nikita Rogalev. All rights reserved.

#include <glad/glad.h>                              // TODO: (УБРАТЬ ОТСЮДА!!!!!!!!!!) Загрузчик OpenGL функций

// Window Class
#include "WindowGLFW.h"                         // Заголовок класса
#include <iostream>
#include "BaseEngine.h"
#include <string>
#include <sstream>
#include <vector>
//#include "Engine/Render/Camera.h"   
#include "Engine/Core/EngineConfig.h"   

#include "Platform/RenderAPI/OpenGL/OpenGLContext.h"   


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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

        // Включение MSAA (многократная выборка)
        glfwWindowHint(GLFW_SAMPLES, 4);

        //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

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
        glfwGetWindowPos(GetHandle(),&m_windowedX,&m_windowedY);

        // Делаем контекст OpenGL текущим для этого окна
        glfwMakeContextCurrent(GetHandle());

        // Включение/выключение вертикальной синхронизации
        glfwSwapInterval(config.vsync ? 1 : 0); //VSync!!!

        // Устанавливаем колбэк изменения
        glfwSetFramebufferSizeCallback(GetHandle(), FramebufferResizeCallback);
        glfwSetWindowFocusCallback(GetHandle(), FocusCallback);
        glfwSetWindowIconifyCallback(GetHandle(), IconifyCallback);

        ENGINE_LOG_INFO("Window ( {} ) has been created successfully", config.title);
        ///////////////////////////////////////////


        m_context = Engine::Render::GraphicsContext::Create(GetHandle());     // Создание рендерера через фабрику (OpenGL, Vulkan и т.д.)
		m_context->Init();                                                 // Инициализация

        //m_currentScene = CreateUniquePtr<Scene>("MainScene");              // Создаём главную сцену с именем "MainScene" и сохраняем её в unique_ptr
        //m_currentScene->SetParentRender(m_render.get());                                            // Устанавливаем для сцены указатель на рендерер (чтобы сцена могла отрисовывать объекты)

        //m_uiSystem = CreateUniquePtr<UISystem>();                         // Создаем систему обработки UI
        //m_uiSystem.get()->Initialize(this);                                // Инициализируем

        // Сохраняем указатель на объект WindowGLF3 в пользовательских данных GLFW, чтобы иметь доступ к нему в статических колбэках.
        glfwSetWindowUserPointer(GetHandle(), this);

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
        glfwSetWindowTitle(GetHandle(),NewNameResult.c_str());
    }

    /**
     * @brief Проверяет, должно ли окно быть закрыто.
     * @return true, если окно запросило закрытие.
     */
    bool WindowGLFW::ShouldClose() const 
    {
        return glfwWindowShouldClose(m_window);
    }

    void WindowGLFW::BeginRender()
    {
        if (m_dirt_width_height)    // В прошлом кадре параметры окна изменились!
        {
            m_dirt_width_height = false;
        }
    }

    void WindowGLFW::EndRender()
    {
        glfwPollEvents();               // Обработка событий (клавиатура, мышь и т.д.)
		m_context->SwapBuffers();   
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

        //if (m_uiSystem.get())
        //{
        //    m_uiSystem->Update();
        //}

        //m_currentScene->update(Time::TimeSystem::GetDeltaTimeSeconds());                                // Обновление логики сцены с передачей deltaTime
    }

    /**
     * @brief Закрывает окно и завершает работу GLFW.
     */
    void WindowGLFW::Close() 
    {
        if (m_window) 
        {
            ENGINE_LOG_INFO("The Window close");
            glfwDestroyWindow(GetHandle());            // Уничтожаем окно GLFW
            m_window = nullptr;
        }
        //if (m_uiSystem)
        //{
        //    m_uiSystem.get()->Shutdown();
        //}
        //m_uiSystem.reset();
        glfwTerminate();                            // Завершаем GLFW (вызывается даже если окно уже было уничтожено)
    }

    /**
     * @brief Инициирует закрытие окна (устанавливает флаг ShouldClose).
     */
    void WindowGLFW::ExitApp()
    {
        ENGINE_CORE_ASSERT(m_window, "FATAL ERROR: NO VALID APPLICATION!");
        glfwSetWindowShouldClose(GetHandle(), true);
    }

    void WindowGLFW::SetWindowMode(WindowMode NewMode)
    {
        if (m_windowMode == NewMode) return;

        m_windowMode = NewMode;
        GLFWmonitor* monitor = nullptr;
        int x = 0, y = 0, w = m_width, h = m_height, refreshRate = 0;

        switch (NewMode)
        {
            case WindowMode::Window:
            {
                monitor = nullptr;                                              // Оконный режим: монитор = nullptr
                x = 100; y = 100;                                               // или сохранённая позиция
                w = m_width; h = m_height;
                refreshRate = 0;                                                // игнорируется
                glfwSetWindowAttrib(GetHandle(), GLFW_DECORATED, GLFW_TRUE);       // Возвращаем рамку окна
                break;
            }
        
            case WindowMode::Borderless:
            {
                monitor = glfwGetPrimaryMonitor();                              // Borderless = фуллскрин на нулевом мониторе, но с оконными координатами
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);            // Получаем размеры рабочего стола (учитывая панель задач)
                int monitorX, monitorY;
                glfwGetMonitorPos(monitor, &monitorX, &monitorY);
            
                x = monitorX; y = monitorY;
                w = mode->width; h = mode->height;
                refreshRate = mode->refreshRate;
                glfwSetWindowAttrib(GetHandle(), GLFW_DECORATED, GLFW_FALSE);      // Убираем рамку, но оставляем окно "оконным" для быстрого переключения
                break;
            }
        
            case WindowMode::Fullscreen:
            {
                monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            
                x = 0; y = 0; // игнорируется в фуллскрине
                w = mode->width; h = mode->height;
                refreshRate = mode->refreshRate; // важно для плавности
            
                glfwSetWindowAttrib(GetHandle(), GLFW_DECORATED, GLFW_FALSE);
                break;
            }
        }

        glfwSetWindowMonitor(GetHandle(), monitor, x, y, w, h, refreshRate);
    
        if (NewMode == WindowMode::Window) 
        {
            m_width = w; m_height = h;
        }

        int fbW, fbH;
        glfwGetFramebufferSize(GetHandle(), &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);
    
        // Возвращаем фокус окну
        glfwFocusWindow(GetHandle());
    
        ENGINE_LOG_INFO("Window mode changed to: {}", (NewMode == WindowMode::Window) ? "Windowed" : (NewMode == WindowMode::Borderless ? "Borderless" : "Fullscreen"));

        if (m_windowMode == WindowMode::Window) 
        {
            glfwSetWindowPos(GetHandle(),m_windowedX,m_windowedY);
        }
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
        auto* win = (class WindowGLFW *)glfwGetWindowUserPointer(window);
        if (win) 
        {
            if (win->GetWindowMode() == WindowMode::Window) 
            {
                win->m_width = width;
                win->m_height = height;
                win->m_dirt_width_height = true;
            }
            
            // Устанавливаем область вывода OpenGL на весь новый размер окна
            glViewport(0, 0, width, height);
            win->OnUpdateWindowSize().Broadcast(window, width, height);
            win->OnWindowReSize().Broadcast(width,height);
            if (win->GetWindowMode() == WindowMode::Window) 
            {
                glfwGetWindowPos(window, &win->m_windowedX, &win->m_windowedY);
            }
        }
    }

    void WindowGLFW::FocusCallback(GLFWwindow *window, int focused)
    {
        auto* win = (class WindowGLFW *)glfwGetWindowUserPointer(window);
        if (win) 
        {
            win->m_WindowHasFocus = (focused == GLFW_TRUE);
            win->OnHasFocusChange().Broadcast(win->m_WindowHasFocus);
        }
    }

    void WindowGLFW::IconifyCallback(GLFWwindow *window, int iconified)
    {
        auto* win = (class WindowGLFW *)glfwGetWindowUserPointer(window);
        if (win) 
        {
            win->m_WindowIsMinimized = (iconified == GLFW_TRUE);
            win->OnMinimizedChange().Broadcast(win->m_WindowIsMinimized);
        }
    }

    bool WindowGLFW::LoadIconFromFile()
    {
        TRef<Texture2D> LIcon = Engine::Render::Texture2D::Create("/Resources/Textures/SredaIco.png");
        ENGINE_ASSERT(LIcon.get(), "Failed to load the icon at path!");
        
        GLFWimage images[1];
        int width, height, channels;

        //unsigned char* data = stbi_load(iconPath, &width, &height, &channels, 4);
        auto data = LIcon->GetDataRGBA8();

        images[0].width = LIcon.get()->GetWidth();
        images[0].height = LIcon.get()->GetHeight();
        images[0].pixels = data.data();

        glfwSetWindowIcon(GetHandle(), 1, images);

        return true;
    }
}
//#endif