// (c) Nikita Rogalev. All rights reserved.

#pragma once // Директива препроцессора для защиты от множественного включения

#include <string>
#include <vector>

namespace Engine
{
    // Слой движка
    class Layer
    {
    public:
        Layer();
		virtual ~Layer() = default;

        virtual void OnInit() {}
        virtual void OnDeInit() {}

        virtual void OnBeginFrame() {}
        virtual void OnRenderFrame() {}
        virtual void OnEndFrame() {}

        virtual void OnUIRender() {}
    private:
        std::string m_name;
        std::string m_category;
    public:
        std::string GetName() {return m_name;}
        std::string GetCategory() {return m_category;}
    };

    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Layer* InLayer);
        void PushOverlay(Layer* InOverlay);
        void PopLayer(Layer* InLayer);
        void PopOverlay(Layer* InOverlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

        void OnStackBeginFrame();
        void OnStackRenderFrame();
        void OnStackEndFrame();
        void OnStackUIRender();

    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };
}