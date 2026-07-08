// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Защита от множественного включения

#ifdef ENGINE_WINDOW_GLFW
#include <GLFW/glfw3.h>
#endif
#include "Engine/Platform/IWindow.h"    // Базовый интерфейс окна
#include "Engine/UI/Framework/UISystem.h"    // Базовый интерфейс окна
#include <string>
#include <functional>
#include "Engine/Core/Utilities/Event.h"

namespace Engine
{
    using namespace Engine::Render;
    ADD_DELEGATE_ONE_PARAM(DOnUpdateWindowHandle, GLFWwindow*)                  // При смене окна
    ADD_DELEGATE_THREE_PARAMS(DOnUpdateWindowSize,GLFWwindow*, int,int)         // При смене размера окна

    using namespace Engine::UI;
    /**
     * @class WindowGLFW
     * @brief Реализация окна с использованием библиотеки GLFW.
     *
     * Наследует абстрактный класс Window и предоставляет конкретные методы
     * для работы с GLFW: создание окна, обработка событий, изменение заголовка,
     * колбэки и т.д.
     */
    class WindowGLFW : public IWindow
    {
    public:
        WindowGLFW();
        ~WindowGLFW() override;

        virtual bool Init(const Engine::WindowConfig& config) override;                 // Инициализация окна
        virtual void UpdateWindowName(std::string NewName) override;                    // Поменять имя окна 
        virtual bool ShouldClose() const override;                                      // Окно должно закрыться
        virtual void BeginRender() override;
        virtual void EndRender() override;
        virtual void Update() override;                                                 // Очистка буферов
        virtual int GetWidth() const override {return m_width;}                         // Получить длину и ширину
        virtual int GetHeight() const override {return m_height;}
        GLFWwindow* GetHandle() {return m_window;}                                // Получить окно
        virtual void Close() override;                                            // Закрываем окно
        virtual void WindowTerminate() override;
        virtual void SetTittle(const std::string NewTittle) override {NameWindow = NewTittle;};
        //virtual Scene* GetCurrentScene() override {return m_currentScene.get();}
        virtual GraphicsContext* GetGraphicsContext() override {return m_GraphicsContext.get();}
        virtual WindowMode GetWindowMode() override {return m_windowMode;};
        virtual void SetWindowMode(WindowMode NewMode) override;
        DOnUpdateWindowHandle& OnUpdateWindowHandle() {return s_OnUpdateWindowHandle;}
        DOnUpdateWindowSize& OnUpdateWindowSize() {return s_OnUpdateWindowSize;}

        virtual bool IsWindowHasFocus() override {return m_WindowHasFocus;};
        virtual bool IsWindowMinimized() override {return m_WindowIsMinimized;};

        static void FocusCallback(GLFWwindow* window, int focused);
        static void IconifyCallback(GLFWwindow* window, int iconified);
        virtual bool LoadIconFromFile() override;
        virtual Engine::WindowContext GetWindowContext() override {return m_WindowContext;}

    private:
        GLFWwindow* m_window = nullptr;                                          // Указатель на объект окна GLFW
        //GLFWwindow* m_window = nullptr;                                                     // Указатель на объект окна GLFW
        static void FramebufferResizeCallback(GLFWwindow* Window, int width, int height);   // Статический колбэк изменения размера фреймбуфера
        int m_width = 0;                                                                    // Текущая ширина окн
        int m_height = 0;                                                                   // Текущая высота окна
        bool m_dirt_width_height = false;                                                   // Была изменена ширина и высота окна, проверить данные в начале кадра!
        std::string NameWindow;                                                             // Заголовок окна (хранится, возможно, для обновления)
        int m_windowedX = 0;                                                                // Координаты расположения окна по X
        int m_windowedY = 0;                                                                // Координаты расположения окна по Y
        WindowMode m_windowMode = WindowMode::Window;
        DOnUpdateWindowHandle s_OnUpdateWindowHandle;
        DOnUpdateWindowSize s_OnUpdateWindowSize;

        bool m_WindowHasFocus = true;
        bool m_WindowIsMinimized = false;
    protected:
        //TUniquePtr<Scene> m_currentScene;               // Текущая сцена
        TUniquePtr<UISystem> m_uiSystem;                  // Текущая система UI
        TUniquePtr<GraphicsContext> m_GraphicsContext;    // Графический контекст (OpenGL, Vulkan и т.д.)
        Engine::WindowContext m_WindowContext = Engine::WindowContext(0);
    };
}

//#endif