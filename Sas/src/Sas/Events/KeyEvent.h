#pragma once
#include "Sas\Events\Event.h"
#include "Sas\Core\KeyCodes.h"
namespace Sas {

	class SAS_API KeyEvent : public Event {
	public:
		KeyCode GetKeyKode() { return m_KeyCode; };
		int GetMode() { return mode; };

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(const KeyCode keycode, const int mode)
			:m_KeyCode(keycode), mode(mode)
		{}
		KeyCode m_KeyCode;
		int mode;
	};

	class SAS_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount, const int mode)
			: KeyEvent(keycode, mode), m_RepeatCount(repeatCount) {}

		uint16_t GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		uint16_t m_RepeatCount;
	};

	class SAS_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode, const int mode)
			: KeyEvent(keycode, mode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class SAS_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode, const int mode)
			: KeyEvent(keycode, mode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};

}