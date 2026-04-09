// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include <memory>
#include <unordered_map>
#include <functional>
#include "Engine/Core/Utilities/Types.h"

namespace Engine::UI 
{

    class UIContext 
    {
    public:
        virtual ~UIContext() = default; 
        

        virtual void InitContext(int w, int h) = 0;

        // Загрузка документа
        virtual bool LoadDocument(const std::string& path) = 0;
        virtual void UnloadDocument() = 0;
        
        // Видимость
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual bool IsVisible() const = 0;

        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void BeginFrame()= 0;
	    virtual void EndFrame()= 0;
    };


    /**
     * @class WindowAPIFactory
     * @brief Фабрика для создания экземпляра окна в зависимости от платформы/API.
     *
     * Скрывает детали конкретной реализации окна за единым интерфейсом Window.
     */
    class UIContextFactory 
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
        static std::unique_ptr<UIContext> create();
    };
}