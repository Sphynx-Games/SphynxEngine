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
		WindowResize, WindowFocusGained, WindowFocusLost, WindowClosed,
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
		Event() : m_IsHandled(false) {}
		virtual ~Event() {}

		inline void SetHandled(bool handled) { m_IsHandled = handled; }
		inline bool IsHandled() { return m_IsHandled; }

		inline bool IsInCategory(EventCategory categories) const { return categories & GetCategory(); }
		virtual int GetCategory() const = 0;
		virtual EventType GetType() const = 0;
		static EventType GetStaticType() { return EventType::None; };

	public:
		bool m_IsHandled;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : m_Event(event) {};
		~EventDispatcher() {};

		template<typename T>
		void Dispatch(std::function<bool(T&)> func)
		{
			if (m_Event.GetType() == T::GetStaticType())
			{
				m_Event.m_IsHandled = func((T&)m_Event);
			}
		}

	private:
		Event& m_Event;
	};
}