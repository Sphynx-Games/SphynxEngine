#pragma once

#include "Event.h"


namespace Sphynx
{
	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(uint32_t windowID, int32_t keycode, bool repeat = false) :
			Event(windowID),
			m_Keycode(keycode),
			m_Repeat(repeat) {}
		virtual ~KeyPressedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::InputEvent | EventCategory::KeyboardEvent);
		EVENT_CLASS_TYPE(EventType::KeyPressed);

		int32_t GetKeycode() const { return m_Keycode; }
		bool IsRepeat() const { return m_Repeat; }

	private:
		int32_t m_Keycode;
		bool m_Repeat;
	};

	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(uint32_t windowID, int32_t keycode) :
			Event(windowID),
			m_Keycode(keycode) {}
		virtual ~KeyReleasedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::InputEvent | EventCategory::KeyboardEvent);
		EVENT_CLASS_TYPE(EventType::KeyReleased);

		int32_t GetKeycode() const { return m_Keycode; }

	private:
		int32_t m_Keycode;
	};

	class KeyTypedEvent : public Event
	{
	public:
		KeyTypedEvent(uint32_t windowID, int32_t keycode, const std::string& text) :
			Event(windowID),
			m_Keycode(keycode),
			m_Text(text) {}
		virtual ~KeyTypedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::InputEvent | EventCategory::KeyboardEvent);
		EVENT_CLASS_TYPE(EventType::KeyTyped);

		int32_t GetKeycode() const { return m_Keycode; }
		const std::string& GetText() const { return m_Text; }

	private:
		int32_t m_Keycode;
		std::string m_Text;
	};

	class MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(uint32_t windowID, int32_t button) :
			Event(windowID),
			m_Button(button) {}
		virtual ~MouseButtonPressedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::InputEvent | EventCategory::MouseEvent);
		EVENT_CLASS_TYPE(EventType::MouseButtonPressed);

		int32_t GetButton() const { return m_Button; }

	private:
		int32_t m_Button;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(uint32_t windowID, int32_t button) :
			Event(windowID),
			m_Button(button) {}
		virtual ~MouseButtonReleasedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::InputEvent | EventCategory::MouseEvent);
		EVENT_CLASS_TYPE(EventType::MouseButtonReleased);

		int32_t GetButton() const { return m_Button; }

	private:
		int32_t m_Button;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(uint32_t windowID, float x, float y) :
			Event(windowID),
			m_ScolledX(x),
			m_ScolledY(y) {}
		virtual ~MouseScrolledEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::InputEvent | EventCategory::MouseEvent);
		EVENT_CLASS_TYPE(EventType::MouseScrolled);

		float GetX() const { return m_ScolledX; };
		float GetY() const { return m_ScolledY; };

	private:
		float m_ScolledX;
		float m_ScolledY;
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(uint32_t windowID, float x, float y) :
			Event(windowID),
			m_PositionX(x),
			m_PositionY(y) {}
		virtual ~MouseMovedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::InputEvent | EventCategory::MouseEvent);
		EVENT_CLASS_TYPE(EventType::MouseMoved);

		float GetX() const { return m_PositionX; };
		float GetY() const { return m_PositionY; };

	private:
		float m_PositionX;
		float m_PositionY;
	};
}