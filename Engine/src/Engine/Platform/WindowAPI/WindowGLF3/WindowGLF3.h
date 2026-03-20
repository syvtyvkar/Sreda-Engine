// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Защита от множественного включения

//#ifdef ENGINE_GLF3
#ifdef ENGINE_GLFW
#include <GLFW/glfw3.h>
#endif
#include "Engine/Platform/IWindow.h"    // Базовый интерфейс окна
#include <string>
#include <functional>


namespace Engine
{
    /**
     * @class WindowGLF3
     * @brief Реализация окна с использованием библиотеки GLFW.
     *
     * Наследует абстрактный класс Window и предоставляет конкретные методы
     * для работы с GLFW: создание окна, обработка событий, изменение заголовка,
     * колбэки и т.д.
     */
    class WindowGLF3 : public Window
    {
    public:
        WindowGLF3();
        ~WindowGLF3() override;

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

    private:
        GLFWwindow* m_window = nullptr;                                                     // Указатель на объект окна GLFW
        static void FramebufferResizeCallback(GLFWwindow* Window, int width, int height);   // Статический колбэк изменения размера фреймбуфера
        int m_width = 0;                                                                    // Текущая ширина окн
        int m_height = 0;                                                                   // Текущая высота окна
        std::string NameWindow;                                                             // Заголовок окна (хранится, возможно, для обновления)
        std::function<void(int, int)> m_resizeCallback = nullptr;                           // Пользовательский колбэк на изменение размера
    };
}

//#endif