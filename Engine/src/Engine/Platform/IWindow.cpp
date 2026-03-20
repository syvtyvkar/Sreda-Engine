// (c) Nikita Rogalev. All rights reserved.

// Window Class
#include "IWindow.h"                                                // Подключаем интерфейс окна (абстрактный класс Window)
#include "Engine/Platform/WindowAPI/WindowGLF3/WindowGLF3.h"        // Конкретная реализация окна для GLFW

namespace Engine 
{

    /**
     * @brief Реализация фабричного метода для создания окна.
     *
     * В зависимости от определённого макроса платформы (здесь ENGINE_GLF3)
     * создаёт и возвращает соответствующий объект окна.
     * Если макрос не определён, возвращает nullptr.
     *
     * @return std::unique_ptr<Window> Уникальный указатель на созданное окно,
     *         либо nullptr, если платформа не поддерживается.
     */
    std::unique_ptr<Window> WindowAPIFactory::create() 
    {
        #ifdef ENGINE_GLF3
        return std::make_unique<WindowGLF3>();  // Создаём окно для GLFW
        #endif
        return nullptr;
    }

}