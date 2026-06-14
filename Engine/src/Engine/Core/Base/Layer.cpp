// (c) Nikita Rogalev. All rights reserved.

#include "Layer.h"                            // Layer class header

namespace Engine
{
    Layer::Layer()
    {

    }

    LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDeInit();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* InLayer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, InLayer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* InOverlay)
	{
		m_Layers.emplace_back(InOverlay);
	}

	void LayerStack::PopLayer(Layer* InLayer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, InLayer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			InLayer->OnDeInit();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* InOverlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), InOverlay);
		if (it != m_Layers.end())
		{
			InOverlay->OnDeInit();
			m_Layers.erase(it);
		}
	}
    void LayerStack::OnStackBeginFrame()
    {
		for (Layer* LL : m_Layers)
		{
			LL->OnBeginFrame();
		}
    }
    void LayerStack::OnStackRenderFrame()
    {
		for (Layer* LL : m_Layers)
		{
			LL->OnRenderFrame();
		}
    }
    void LayerStack::OnStackEndFrame()
    {
		for (Layer* LL : m_Layers)
		{
			LL->OnEndFrame();
		}
    }
    void LayerStack::OnStackUIRender()
    {
		for (Layer* LL : m_Layers)
		{
			LL->OnUIRender();
		}
    }
}
