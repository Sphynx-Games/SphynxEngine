#pragma once

#include "Event.h"
#include <cstdint>

namespace Sphynx
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}
		virtual ~WindowResizeEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowResize);

		uint32_t GetWidth() const { return m_Width; };
		uint32_t GetHeight() const { return m_Height; };

	private:
		uint32_t m_Width;
		uint32_t m_Height;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() {}
		virtual ~WindowClosedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowClosed);
	};

	class WindowFocusGainedEvent : public Event
	{
	public:
		WindowFocusGainedEvent() {}
		virtual ~WindowFocusGainedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowFocusGained);
	};

	class WindowFocusLostEvent : public Event
	{
	public:
		WindowFocusLostEvent() {}
		virtual ~WindowFocusLostEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowFocusLost);
	};
}