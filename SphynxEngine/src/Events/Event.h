#pragma once

#include <functional>

#define EVENT_CLASS_CATEGORY(categories)	virtual int GetCategory() const override { return categories; }
#define EVENT_CLASS_TYPE(type)				virtual EventType GetType() const override { return type; };\
											static EventType GetStaticType() { return type; }
namespace Sphynx
{
	enum class EventType
	{
		None,
		WindowResized, WindowFocusGained, WindowFocusLost, WindowClosed,
		WindowShown, WindowHidden, WindowMoved,
		WindowMinimized, WindowMaximized, WindowRestored, WindowFullcreenChanged,
		WindowMouseEnter, WindowMouseExit,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseScrolled, MouseMoved
	};

	enum EventCategory
	{
		None = 1 << 0,
		WindowEvent = 1 << 1,
		InputEvent = 1 << 2,
		MouseEvent = 1 << 3,
		KeyboardEvent = 1 << 4,
	};

	class Event
	{
	public:
		Event(uint32_t windowID) : 
			m_IsHandled(false),
			m_WindowID(windowID) {}
		virtual ~Event() {}

		inline void SetHandled(bool handled) { m_IsHandled = handled; }
		inline bool IsHandled() { return m_IsHandled; }
		inline uint32_t GetWindowID() const { return m_WindowID; }

		inline bool IsInCategory(EventCategory categories) const { return categories & GetCategory(); }
		virtual int GetCategory() const = 0;
		virtual EventType GetType() const = 0;
		static EventType GetStaticType() { return EventType::None; };

	private:
		bool m_IsHandled;
		uint32_t m_WindowID;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : m_Event(event) {};
		~EventDispatcher() {};

		template<typename TEvent>
		void Dispatch(std::function<bool(TEvent&)> func)
		{
			static_assert(std::is_base_of_v<Event, TEvent>, "TEvent should be an event class");

			if (m_Event.GetType() == TEvent::GetStaticType())
			{
				m_Event.SetHandled(func((TEvent&)m_Event));
			}
		}

	private:
		Event& m_Event;
	};
}