#pragma once

#include "Hazel/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void Update();

		inline void PushLayer(Layer* layer) { m_ToAddLayer.emplace_back(layer); }
		inline void PushOverlay(Layer* overlay) { m_ToAddOverlay.emplace_back(overlay); }
		inline void PopLayer(Layer* layer) { m_ToRemoveLayer.emplace_back(layer); }
		inline void PopOverlay(Layer* overlay) { m_ToRemoveOverlay.emplace_back(overlay); }

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		void PushLayerImpl(Layer* layer);
		void PushOverlayImpl(Layer* overlay);
		void PopLayerImpl(Layer* layer);
		void PopOverlayImpl(Layer* overlay);


	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;

		std::vector<Layer*> m_ToAddLayer, m_ToAddOverlay;
		std::vector<Layer*> m_ToRemoveLayer, m_ToRemoveOverlay;

	};

}
