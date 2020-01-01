#pragma once

#include "Event.h"

namespace Hazel {

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class TextTypedEvent : public Event
	{
	public:
		TextTypedEvent(const char* text)
			: m_Text(text) {}

		inline const char* GetText() const { return m_Text; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "TextTypedEvent: " << m_Text;
			return ss.str();
		}

		EVENT_CLASS_TYPE(TextTyped)

	protected:

		const char* m_Text;
	};
}
