#pragma once

#include "Container/Array.h"


namespace Sphynx
{
	class Layer;

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline const Array<Layer*>& get() { return m_Layers; }
		inline size_t size() { return m_Layers.Size(); }

		Array<Layer*>::Iterator begin() { return m_Layers.begin(); }
		Array<Layer*>::Iterator end() { return m_Layers.end(); }
		Array<Layer*>::ReverseIterator rbegin() { return m_Layers.rbegin(); }
		Array<Layer*>::ReverseIterator rend() { return m_Layers.rend(); }

		Array<Layer*>::ConstIterator begin() const { return m_Layers.begin(); }
		Array<Layer*>::ConstIterator end()	const { return m_Layers.end(); }
		Array<Layer*>::ConstReverseIterator rbegin() const { return m_Layers.rbegin(); }
		Array<Layer*>::ConstReverseIterator rend() const { return m_Layers.rend(); }

	private:
		Array<Layer*> m_Layers;
		uint64_t m_LayerInsertIndex;
	};
}