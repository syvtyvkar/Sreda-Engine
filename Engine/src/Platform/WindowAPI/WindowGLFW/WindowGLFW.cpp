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
#include "Engine/Render/RendererAPI.h"
#include "Engine/Render/RenderAPIFactory.h"

#include "Platform/RenderAPI/OpenGL/OpenGLContext.h"   


namespace Engine
{
    static GLFWwindow* s_SharedWindow = nullptr;  // First window's handle for context sharing

    WindowGLFW::WindowGLFW() =default;          // Конструктор по умолчанию

    WindowGLFW::~WindowGLFW()                   // Деструктор: автоматически закрывает окно при уничтожении объекта
    {
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
        m_WindowContext = config.cntx;

        bool isVulkan = false;
        
        switch (Engine::Render::RenderAPIFactory::GetRenderAPI())
        {
        case Engine::Render::RenderAPIFactory::RHI_API::Vulkan:
            isVulkan = true;
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            break;
        case Engine::Render::RenderAPIFactory::RHI_API::OpenGL:
            // OpenGL (4.5 Core Profile)
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef DEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        #endif

            // Включение MSAA (многократная выборка)
            glfwWindowHint(GLFW_SAMPLES, 4);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
            break;
        default:
            ENGINE_ASSERT(false, "No valid Render API!");
            break;
        }

        m_window = glfwCreateWindow(config.wight,config.height,config.title.c_str(),nullptr,s_SharedWindow);  // Создание окна GLFW

        if (!m_window) 
        {
            ENGINE_LOG_CRITICAL("Failed to create GLFW window");
            glfwTerminate();                                        // Завершаем GLFW, если окно не создано
            return false;
        }

        // Сохраняем размеры окна
        m_width = config.wight;
        m_height = config.height;
        glfwGetWindowPos(GetHandle(),&m_windowedX,&m_windowedY);

        if (!isVulkan)
        {
            // Делаем контекст OpenGL текущим для этого окна
            glfwMakeContextCurrent(GetHandle());

            // Включение/выключение вертикальной синхронизации
            glfwSwapInterval(config.vsync ? 1 : 0); //VSync!!!
        }

        ENGINE_LOG_INFO("Window ( {} ) has been created successfully", config.title);
        ///////////////////////////////////////////

        m_GraphicsContext = RenderAPIFactory::CreateGraphicsContext(shared_from_this());
		m_GraphicsContext->Init();                                                 // Инициализация

        if (!s_SharedWindow)
            s_SharedWindow = GetHandle();

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
        NewNameResult.append(RenderAPIFactory::GetNameRenderAPI() + " ");
        NewNameResult.append("[" + NewName + " FPS]");
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
       // m_GraphicsContext->MakeCurrent();
        if (!m_GraphicsContext) return;
        m_GraphicsContext->BeginFrame();

        if (m_dirt_width_height)    // В прошлом кадре параметры окна изменились!
        {
            m_dirt_width_height = false;
        }
    }

    void WindowGLFW::EndRender()
    {
        glfwPollEvents();               // Обработка событий (клавиатура, мышь и т.д.)
        if (!m_GraphicsContext) return;
		m_GraphicsContext->EndFrame();
		m_GraphicsContext->SwapBuffers();
        //glfwSwapBuffers(GetHandle());   
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

        //m_currentScene->update(Time::TimeSystem::GetDeltaTimeSeconds());                                // Обновление логики сцены с передачей deltaTime
    }

    /**
     * @brief Закрывает окно и завершает работу GLFW.
     */
    void WindowGLFW::Close() 
    {
        ENGINE_CORE_ASSERT(m_window, "FATAL ERROR: NO VALID WINDOW!");

        OnHasFocusChange().Clear();
        OnMinimizedChange().Clear();
        OnWindowReSize().Clear();
        OnWindowClose().Clear();

        if (GetHandle() == s_SharedWindow)
            s_SharedWindow = nullptr;

        ENGINE_LOG_INFO("The Window {0} close", GetWindowContext().WindowID);
        //glfwSetWindowShouldClose(GetHandle(), true);
        //glfwDestroyWindow(GetHandle());            // Уничтожаем окно GLFW
        
        m_GraphicsContext.reset();
        glfwDestroyWindow(GetHandle()); 
        glfwSetWindowShouldClose(GetHandle(), true);
    }

    void WindowGLFW::WindowTerminate()
    {
        glfwTerminate();
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
        if (Engine::Render::RenderAPIFactory::GetRenderAPI() != Engine::Render::RenderAPIFactory::RHI_API::Vulkan)
            glViewport(0, 0, fbW, fbH);
    
        // Возвращаем фокус окну
        glfwFocusWindow(GetHandle());
    
        ENGINE_LOG_INFO("Window mode changed to: {}", (NewMode == WindowMode::Window) ? "Windowed" : (NewMode == WindowMode::Borderless ? "Borderless" : "Fullscreen"));

        if (m_windowMode == WindowMode::Window) 
        {
            glfwSetWindowPos(GetHandle(),m_windowedX,m_windowedY);
        }

        OnWindowModeChange().Broadcast(GetWindowContext(), m_windowMode);
    }

    bool WindowGLFW::LoadIconFromFile()
    {
        TRef<Texture2D> LIcon = RenderAPIFactory::CreateTexture2DFromPath("/Resources/Textures/SredaIco.png");
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

    void WindowGLFW::FocusThisWindow()
    {
        if (GetHandle() == glfwGetCurrentContext()) return;
        if (!m_GraphicsContext) return;
        
        m_GraphicsContext->MakeCurrent();
    }
}
//#endif