#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers) {
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::Update()
	{
		for (Layer* layer : m_ToAddLayer)
			PushLayerImpl(layer);
		for (Layer* layer : m_ToAddOverlay)
			PushOverlayImpl(layer);
		for (Layer* layer : m_ToRemoveLayer)
			PopLayerImpl(layer);
		for (Layer* layer : m_ToRemoveOverlay)
			PopOverlayImpl(layer);
		m_ToAddLayer.clear(); m_ToAddOverlay.clear(); m_ToRemoveLayer.clear(); m_ToRemoveOverlay.clear();
	}

	void LayerStack::PushLayerImpl(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		layer->OnAttach();
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlayImpl(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayerImpl(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.end())
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlayImpl(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			overlay->OnDetach();
		}
	}

}