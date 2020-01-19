#include "hzpch.h"

#include "Event.h"
#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace Hazel {
	void Hazel::Event::ClearAllocations()
	{//Clears all of the allocation pools for each event type
		MemoryPool<sizeof(WindowCloseEvent)>::Clear();
		MemoryPool<sizeof(WindowResizeEvent)>::Clear();

		MemoryPool<sizeof(AppTickEvent)>::Clear();
		MemoryPool<sizeof(AppUpdateEvent)>::Clear();
		MemoryPool<sizeof(AppRenderEvent)>::Clear();

		MemoryPool<sizeof(KeyPressedEvent)>::Clear();
		MemoryPool<sizeof(KeyReleasedEvent)>::Clear();
		MemoryPool<sizeof(TextTypedEvent)>::Clear();

		MemoryPool<sizeof(MouseButtonPressedEvent)>::Clear();
		MemoryPool<sizeof(MouseButtonReleasedEvent)>::Clear();
		MemoryPool<sizeof(MouseMovedEvent)>::Clear();
		MemoryPool<sizeof(MouseScrolledEvent)>::Clear();
	}

}
