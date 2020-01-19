#pragma once

#include <stdint.h>
#include <sstream>

#include "Hazel/Events/EventPool.h"
#include "Hazel/Core/Core.h"

namespace Hazel {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, TextTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4)
	};


#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return EventType::type; }						\
								virtual EventType GetEventType() const override { return GetStaticType(); }			\
								virtual const char* GetName() const override { return #type; }						\
																													\
								static void* operator new (std::size_t size)												\
								{																					\
									MemoryPool<sizeof(type##Event)>::Init();										\
									return MemoryPool<sizeof(type##Event)>::Allocate();								\
								}																					\
								static void operator delete (void* ptr)												\
								{																					\
									HZ_CORE_TRACE("Trying to delete event");										\
								}																					\



#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		static void ClearAllocations();

	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		virtual ~Event() {}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event* event)
			: m_Event(event)
		{
		}

		template<typename T, typename F>
		bool Dispatch(F&& func)
		{
			if (m_Event->GetEventType() == T::GetStaticType())
			{
				m_Event->Handled = std::forward<F>(func)(static_cast<T*>(m_Event));
				return true;
			}
			return false;
		}

		template<typename T, typename C, typename F>
		bool DispatchInstance(F&& func, C* instance)
		{
			if (m_Event->GetEventType() == T::GetStaticType())
			{
				m_Event->Handled = (instance->*func)(static_cast<T*>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event* m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}

