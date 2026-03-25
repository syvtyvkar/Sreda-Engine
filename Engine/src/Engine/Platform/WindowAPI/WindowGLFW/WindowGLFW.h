// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Защита от множественного включения

#ifdef ENGINE_WINDOW_GLFW
#include <GLFW/glfw3.h>
#endif
#include "Engine/Platform/IWindow.h"    // Базовый интерфейс окна
#include <string>
#include <functional>


namespace Engine
{
    /**
     * @class WindowGLFW
     * @brief Реализация окна с использованием библиотеки GLFW.
     *
     * Наследует абстрактный класс Window и предоставляет конкретные методы
     * для работы с GLFW: создание окна, обработка событий, изменение заголовка,
     * колбэки и т.д.
     */
    class WindowGLFW : public Window
    {
    public:
        WindowGLFW();
        ~WindowGLFW() override;

        virtual bool Init(const Engine::WindowConfig& config) override;                 // Инициализация окна
        virtual void UpdateWindowName(std::string NewName) override;                    // Поменять имя окна 
        virtual bool ShouldClose() const override;                                      // Окно должно закрыться
        virtual void Update() override;                                                 // Очистка буферов
        virtual int GetWidth() const override {return m_width;}                         // Получить длину и ширину
        virtual int GetHeight() const override {return m_height;}
        GLFWwindow* GetHandle() {return m_window;}                                      // Получить окно
        virtual void SetResizeCallback(std::function<void(int,int)>callback) override;  // Установить делегат для изменения окна 
        virtual void Close() override;                                                  // Закрываем окно
        virtual void ExitApp() override;                                                // Закрываем окно
        virtual Scene* GetCurrentScene() override {return m_currentScene.get();}
        virtual RenderAPI* GetCurrentRender() override {return m_render.get();}
        virtual WindowMode GetWindowMode() override {return m_windowMode;};
        virtual void SetWindowMode(WindowMode NewMode) override;
    private:
        GLFWwindow* m_window = nullptr;                                                     // Указатель на объект окна GLFW
        static void FramebufferResizeCallback(GLFWwindow* Window, int width, int height);   // Статический колбэк изменения размера фреймбуфера
        int m_width = 0;                                                                    // Текущая ширина окн
        int m_height = 0;                                                                   // Текущая высота окна
        std::string NameWindow;                                                             // Заголовок окна (хранится, возможно, для обновления)
        std::function<void(int, int)> m_resizeCallback = nullptr;                           // Пользовательский колбэк на изменение размера
        WindowMode m_windowMode = WindowMode::Window;
    protected:
        std::unique_ptr<RenderAPI> m_render;                    // Рендерер (OpenGL, Vulkan и т.д.)
        std::unique_ptr<Scene> m_currentScene;                  // Текущая сцена
    };
}

//#endif