#pragma once

#include "Event.h"
#include <cstdint>

namespace Sphynx
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}
		virtual ~WindowResizedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowResized);

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

	class WindowShownEvent : public Event
	{
	public:
		WindowShownEvent() {}
		virtual ~WindowShownEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowShown);
	};

	class WindowHiddenEvent : public Event
	{
	public:
		WindowHiddenEvent() {}
		virtual ~WindowHiddenEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowHidden);
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(uint32_t x, uint32_t y) :m_PositionX(x), m_PositionY(y) {}
		virtual ~WindowMovedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowMoved);

		inline uint32_t GetPositionX() const { return m_PositionX; }
		inline uint32_t GetPositionY() const { return m_PositionY; }

	private:
		uint32_t m_PositionX;
		uint32_t m_PositionY;
	};

	class WindowMinimizedEvent : public Event
	{
	public:
		WindowMinimizedEvent() {}
		virtual ~WindowMinimizedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowMinimized);
	};

	class WindowMaximizedEvent : public Event
	{
	public:
		WindowMaximizedEvent() {}
		virtual ~WindowMaximizedEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowMaximized);
	};

	class WindowRestoredEvent : public Event
	{
	public:
		WindowRestoredEvent() {}
		virtual ~WindowRestoredEvent() {};

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowRestored);
	};

	class WindowFullscreenChangedEvent : public Event
	{
	public:
		WindowFullscreenChangedEvent(bool fullscreen) : m_IsFullscreen(fullscreen) {}
		virtual ~WindowFullscreenChangedEvent() {}

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowFullcreenChanged);

		bool IsFullscreen() const { return m_IsFullscreen; }

	private:
		bool m_IsFullscreen;
	};

	class WindowMouseEnterEvent : public Event
	{
	public:
		WindowMouseEnterEvent() {}
		virtual ~WindowMouseEnterEvent() {}

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowMouseEnter);
	};

	class WindowMouseExitEvent : public Event
	{
	public:
		WindowMouseExitEvent() {}
		virtual ~WindowMouseExitEvent() {}

		EVENT_CLASS_CATEGORY(EventCategory::WindowEvent);
		EVENT_CLASS_TYPE(EventType::WindowMouseExit);
	};
}