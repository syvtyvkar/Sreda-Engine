// (c) Nikita Rogalev. All rights reserved.

#include "Engine/Core/Log.h"                                        // Подключение системы логирования
#include "IInputListen.h"                                            // Заголовок с определением интерфейса InputListen и фабрики InputListenAPIFactory
#include "Engine/Input/InputAPI/InputGLFW/InputGLFW.h"              // Конкретная реализация InputListen для GLFW

namespace Engine 
{
    /**
     * @brief Создаёт экземпляр класса InputListen в зависимости от платформы/API.
     *
     * Реализация фабричного метода. В зависимости от макросов платформы
     * (например, ENGINE_WINDOW_GLFW) возвращает соответствующий объект, реализующий
     * интерфейс InputListen.
     *
     * @return TUniquePtr<InputListen> Уникальный указатель на созданный объект.
     *         Если ни одна платформа не определена, возвращает nullptr.
     */
    TUniquePtr<IInputListen> InputListenAPIFactory::create()
    {
        #ifdef ENGINE_WINDOW_GLFW
        // Если используется GLFW, создаём слушатель ввода, специфичный для GLFW.
        return std::make_unique<InputListenGLFWSystem>();
        #endif
        // В случае отсутствия поддерживаемой платформы возвращаем пустой указатель.
        // В будущем здесь можно добавить другие ветки (например, для Windows Raw Input).
        return nullptr;
    }
}