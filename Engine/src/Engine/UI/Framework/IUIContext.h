// (c) Nikita Rogalev. All rights reserved.

#pragma once    // Защита от множественного включения

#include <memory>
#include <unordered_map>
#include <functional>
#include "Engine/Core/Utilities/Types.h"
#include "Engine/Platform/IWindow.h"
#include "Engine/UI/Framework/UIElement.h"

namespace Engine::UI 
{

    using namespace Engine;
    class UIContext 
    {
    public:
        virtual ~UIContext() = default; 
        

        virtual void InitContext(Window* window) = 0;

        // Загрузка документа
        virtual bool LoadDocument(const std::string& path) = 0;
        virtual void UnloadDocument() = 0;

        // UI Element Management
        void RegisterWidget(std::shared_ptr<UIElement> widget);
        void RemoveWidget(const std::shared_ptr<UIElement>& widget);
    
        // Root element management
        void SetRootWidget(std::shared_ptr<UIElement> root);
        std::shared_ptr<UIElement> GetRootWidget() const { return m_rootWidget; }
        
        // Видимость
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual bool IsVisible() const = 0;

        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void BeginFrame()= 0;
	    virtual void EndFrame()= 0;

    private:
        void RenderUIElements(std::shared_ptr<UIElement> element);

        std::shared_ptr<UIElement> m_rootWidget;
        std::vector<std::shared_ptr<UIElement>> m_widgets;
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
         * @return TUniquePtr<Window> Указатель на созданное окно.
         *
         * В зависимости от макросов (например, ENGINE_WINDOW_GLFW) возвращает соответствующий
         * наследник Window (например, WindowGLF3). Если ни одна платформа не определена,
         * может вернуть nullptr.
         */
        static TUniquePtr<UIContext> create();
    };
}