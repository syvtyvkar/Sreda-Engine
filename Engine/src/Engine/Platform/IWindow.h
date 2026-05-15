// (c) Nikita Rogalev. All rights reserved.

#pragma once                            // Защита от множественного включения заголовочного файла

#include <string>                                   // Для std::string
#include <functional>                               // Для std::function (используется в колбэке изменения размера)
#include <memory>                                   // Для std::unique_ptr
#include "Engine/Render/GraphicsContext.h"      // Графический контекст рендера
//#include "Engine/Scene/Scene.h"                     // Класс сцены, содержащей игровые объекты
#include "Engine/Core/Utilities/Event.h"

namespace Engine
{

    ADD_DELEGATE_TWO_PARAMS(DOnWindowReSize, int,int)         // При смене размера окна
    ADD_DELEGATE_ONE_PARAM(DOnWindowFocusChange, bool)        // Смена фокуса/сворачивании игры

    using namespace Engine::Render;
    /**
     * @struct WindowConfig
     * @brief Структура, содержащая параметры конфигурации окна при создании.
     * 
     * TODO: ПЕРЕВЕСТИ В КОНФИГИ!
     */
    struct WindowConfig
    {
        int wight = 800;                        // Ширина окна в пикселях
        int height = 600;                       // Высота окна в пикселях
        std::string title = "Render Window";    // Заголовок окна
        bool resizable = true;                  // Флаг, разрешающий изменение размера окна пользователем
        bool vsync = false;                     // Включение вертикальной синхронизации
    };

    /**
     * @struct WindowMode
     * @brief Режимы окна
     * 
     */
    enum class WindowMode : uint8_t 
    {
        Window = 0,
        Borderless = 1,
        Fullscreen = 2
    };

    /**
     * @class Window
     * @brief Абстрактный базовый класс для платформозависимого окна.
     *
     * Определяет интерфейс для работы с окном приложения: создание, обновление,
     * обработка событий, изменение заголовка, получение размеров и т.д.
     * Конкретные реализации (например, для GLFW, SDL, WinAPI) должны наследовать этот класс.
     */
    class Window
    {
    public:
        virtual ~Window()  = default;                                               // Виртуальный деструктор для корректного удаления наследников
        /**
         * @brief Инициализирует окно с заданной конфигурацией.
         * @param config Параметры окна (ширина, высота, заголовок и т.д.)
         * @return true в случае успеха, false при ошибке.
         */
        virtual bool Init(const WindowConfig& config)= 0;
        /**
         * @brief Изменяет заголовок окна.
         * @param NewName Новый заголовок.
         */
        virtual void UpdateWindowName(std::string NewName)= 0;
        /**
         * @brief Проверяет, должно ли окно быть закрыто (например, пользователь нажал крестик).
         * @return true, если окно следует закрыть.
         */
        virtual bool ShouldClose() const= 0;
        /**
         * @brief Подготовока рендера
         * Вызывается каждый кадр в главном цикле приложения.
         */
        virtual void BeginRender()= 0;
        /**
         * @brief Конец отрисовки
         * Вызывается каждый кадр в главном цикле приложения.
         */
        virtual void EndRender()= 0;
        /**
         * @brief Обновляет состояние окна: обрабатывает события, переключает буферы и т.д.
         * Вызывается каждый кадр в главном цикле приложения.
         */
        virtual void Update()= 0;
        /**
         * @brief Возвращает текущую ширину окна.
         * @return Ширина в пикселях.
         */
        virtual int GetWidth() const = 0;
        /**
         * @brief Возвращает текущую высоту окна.
         * @return Высота в пикселях.
         */
        virtual int GetHeight() const = 0;
        /**
         * @brief Показывает текущий режим окна
         */
        virtual WindowMode GetWindowMode() = 0;
        /**
         * @brief Меняет текущий режим окна
         */
        virtual void SetWindowMode(WindowMode NewMode) = 0;
        /**
         * @brief Закрывает окно (устанавливает флаг ShouldClose в true).
         */
        virtual void Close()= 0;     
        /**
         * @brief Немедленно завершает приложение (закрывает окно и выходит из цикла).
         *        Отличается от Close() тем, что может дополнительно инициировать выход.
         */                                           
        virtual void ExitApp()= 0;
        //virtual Scene* GetCurrentScene() = 0;
        virtual GraphicsContext* GetGraphicsContext() = 0;
        virtual void SetTittle(const std::string NewTittle) = 0;
        DOnWindowReSize& OnWindowReSize() {return s_OnWindowReSize;}
        DOnWindowFocusChange& OnHasFocusChange() {return s_OnHasFocusChange;}
        DOnWindowFocusChange& OnMinimizedChange() {return s_OnMinimizedChange;}

        virtual bool IsWindowHasFocus() = 0;
        virtual bool IsWindowMinimized() = 0;
    private:
        DOnWindowReSize s_OnWindowReSize;
        DOnWindowFocusChange s_OnHasFocusChange;
        DOnWindowFocusChange s_OnMinimizedChange;
    };

    /**
     * @class WindowAPIFactory
     * @brief Фабрика для создания экземпляра окна в зависимости от платформы/API.
     *
     * Скрывает детали конкретной реализации окна за единым интерфейсом Window.
     */
    class WindowAPIFactory 
    {
    public:
    /**
         * @brief Создаёт уникальный указатель на объект окна.
         * @return TUniquePtr<Window> Указатель на созданное окно.
         *
         * В зависимости от макросов (например, ENGINE_WINDOW_GLFW) возвращает соответствующий
         * наследник Window (например, WindowGLF3). Если ни одна платформа не определена,
         * может вернуть nullptr.
         */
        static TUniquePtr<Window> create();
    };
}