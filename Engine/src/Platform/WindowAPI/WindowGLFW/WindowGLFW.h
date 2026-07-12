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
        virtual ~WindowGLFW() override;

        virtual bool Init(const Engine::WindowConfig& config) override;                 // Инициализация окна
        virtual void UpdateWindowName(std::string NewName) override;                    // Поменять имя окна 
        virtual bool ShouldClose() const override;                                      // Окно должно закрыться
        virtual void BeginRender() override;
        virtual void EndRender() override;
        virtual void Update() override;                                                 // Очистка буферов
        virtual int GetWidth() const override {return m_width;}                         // Получить длину и ширину
        virtual int GetHeight() const override {return m_height;}
        GLFWwindow* GetHandle() {return m_window;}                                // Получить окно
        virtual void* GetWindowAPIHandle() override {return GetHandle();};
        virtual void Close() override;                                            // Закрываем окно
        virtual void WindowTerminate() override;
        virtual void SetTittle(const std::string NewTittle) override {NameWindow = NewTittle;};
        //virtual Scene* GetCurrentScene() override {return m_currentScene.get();}
        virtual GraphicsContext* GetGraphicsContext() override {return m_GraphicsContext.get();}
        virtual WindowMode GetWindowMode() override {return m_windowMode;};
        virtual void SetWindowMode(WindowMode NewMode) override;

        virtual bool IsWindowHasFocus() override {return m_WindowHasFocus;}
        virtual bool IsWindowMinimized() override {return m_WindowIsMinimized;}

        virtual void SetWindowHasFocus(bool InNewVal) override {m_WindowHasFocus = InNewVal;}
        virtual void SetWindowMinimized(bool InNewVal) override {m_WindowIsMinimized = InNewVal;}

        static void FocusCallback(GLFWwindow* window, int focused);
        static void IconifyCallback(GLFWwindow* window, int iconified);
        static void CloseWindowCallback(GLFWwindow* window);
        virtual bool LoadIconFromFile() override;
        virtual Engine::WindowContext GetWindowContext() override {return m_WindowContext;}

        virtual void FocusThisWindow() override;

        virtual Vector2 GetSizeWindow() const override {return Vector2(m_width,m_height);}
        virtual Vector2 GetLocationWindow() const override {return Vector2(m_windowedX,m_windowedY);}
        virtual void SetSizeWindow(Vector2 InNewVal) override {m_windowedX = InNewVal.x; m_windowedY = InNewVal.y; m_dirt_width_height = true;}
        virtual void SetLocationWindow(Vector2 InNewVal) override {m_width = InNewVal.x; m_height = InNewVal.y;}

    private:
        GLFWwindow* m_window = nullptr;                                                     // Указатель на объект окна GLFW
        static void FramebufferResizeCallback(GLFWwindow* Window, int width, int height);   // Статический колбэк изменения размера фреймбуфера
        int m_width = 0;                                                                    // Текущая ширина окн
        int m_height = 0;                                                                   // Текущая высота окна
        bool m_dirt_width_height = false;                                                   // Была изменена ширина и высота окна, проверить данные в начале кадра!
        std::string NameWindow;                                                             // Заголовок окна (хранится, возможно, для обновления)
        int m_windowedX = 0;                                                                // Координаты расположения окна по X
        int m_windowedY = 0;                                                                // Координаты расположения окна по Y
        WindowMode m_windowMode = WindowMode::Window;

        bool m_WindowHasFocus = true;
        bool m_WindowIsMinimized = false;
    protected:
        //TUniquePtr<Scene> m_currentScene;               // Текущая сцена
        TRef<GraphicsContext> m_GraphicsContext;    // Графический контекст (OpenGL, Vulkan и т.д.)
        Engine::WindowContext m_WindowContext = Engine::WindowContext(0);
    };
}

//#endif