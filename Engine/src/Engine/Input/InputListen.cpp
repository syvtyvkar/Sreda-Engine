// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Core/Log.h"                                        // Подключение системы логирования
#include "InputListen.h"                                            // Заголовок с определением интерфейса InputListen и фабрики InputListenAPIFactory
#include "Engine/Input/InputAPI/InputGLF3/InputGLF3.h"              // Конкретная реализация InputListen для GLFW

namespace Engine::Input 
{
    /**
     * @brief Создаёт экземпляр класса InputListen в зависимости от платформы/API.
     *
     * Реализация фабричного метода. В зависимости от макросов платформы
     * (например, ENGINE_GLFW) возвращает соответствующий объект, реализующий
     * интерфейс InputListen.
     *
     * @return std::unique_ptr<InputListen> Уникальный указатель на созданный объект.
     *         Если ни одна платформа не определена, возвращает nullptr.
     */
    std::unique_ptr<InputListen> InputListenAPIFactory::create()
    {
        #ifdef ENGINE_GLFW
        // Если используется GLFW, создаём слушатель ввода, специфичный для GLFW.
        return std::make_unique<InputListenGLF3System>();
        #endif
        // В случае отсутствия поддерживаемой платформы возвращаем пустой указатель.
        // В будущем здесь можно добавить другие ветки (например, для Windows Raw Input).
        return nullptr;
    }
}