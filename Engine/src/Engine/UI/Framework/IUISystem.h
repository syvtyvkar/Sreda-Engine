// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include "Engine/Platform/IWindow.h"
#include "Engine/UI/Framework/IUIContext.h"
#include "Engine/Core/Utilities/Types.h"
#include <memory>
#include <unordered_map>
#include <functional>

namespace Engine::UI 
{
    class IUISystem 
    {
    public:
        virtual bool Initialize(Window* window) = 0;
        virtual void Shutdown() = 0;
        
        // Обновление и рендер (вызывать в главном цикле)
        virtual void Update()= 0;
        virtual void Render()= 0;
        virtual void BeginFrame()= 0;
	    virtual void EndFrame()= 0;
        
        // Управление контекстами
        virtual UIContext* CreateContext()= 0;
        virtual UIContext* GetContext()= 0;
        virtual void DestroyContext()= 0;
        
        // Отладка
        virtual void ToggleDebugger()= 0;

        virtual ~IUISystem() = default; 
    };

    class UISystemFactory 
    {
    public:
    /**
         * @brief Создаёт уникальный указатель на объект окна.
         * @return std::unique_ptr<Window> Указатель на созданное окно.
         *
         * В зависимости от макросов (например, ENGINE_WINDOW_GLFW) возвращает соответствующий
         * наследник Window (например, WindowGLF3). Если ни одна платформа не определена,
         * может вернуть nullptr.
         */
        static std::unique_ptr<IUISystem> create();
    };

}