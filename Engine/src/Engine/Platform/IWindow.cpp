// (c) Nikita Rogalev. All rights reserved.

// Window Class
#include "IWindow.h"                                                // Подключаем интерфейс окна (абстрактный класс Window)
#include "Engine/Platform/WindowAPI/WindowGLFW/WindowGLFW.h"        // Конкретная реализация окна для GLFW

namespace Engine 
{

    /**
     * @brief Реализация фабричного метода для создания окна.
     *
     * В зависимости от определённого макроса платформы (здесь ENGINE_WINDOW_GLFW)
     * создаёт и возвращает соответствующий объект окна.
     * Если макрос не определён, возвращает nullptr.
     *
     * @return std::unique_ptr<Window> Уникальный указатель на созданное окно,
     *         либо nullptr, если платформа не поддерживается.
     */
    std::unique_ptr<Window> WindowAPIFactory::create() 
    {
        #ifdef ENGINE_WINDOW_GLFW
        return std::make_unique<WindowGLFW>();  // Создаём окно для GLFW
        #endif
        return nullptr;
    }

}