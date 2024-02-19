#pragma once
#include "Event.h"

namespace Sphynx
{
	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(int32_t keycode, bool repeat = false) : m_Keycode(keycode), m_Repeat(repeat) {}
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
		KeyReleasedEvent(int32_t keycode) : m_Keycode(keycode) {}
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
		KeyTypedEvent(int32_t keycode) : m_Keycode(keycode) {}
		virtual ~KeyTypedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::InputEvent | EventCategory::KeyboardEvent);
		EVENT_CLASS_TYPE(EventType::KeyTyped);

		int32_t GetKeycode() const { return m_Keycode; }

	private:
		int32_t m_Keycode;
	};

	class MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(int32_t button) : m_Button(button) {}
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
		MouseButtonReleasedEvent(int32_t button) : m_Button(button) {}
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
		MouseScrolledEvent(float x, float y) : m_ScolledX(x), m_ScolledY(y) {}
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
		MouseMovedEvent(float x, float y) : m_PositionX(x), m_PositionY(y) {}
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