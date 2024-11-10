#pragma once


namespace Sphynx
{
	class Event;

	class Widget
	{
	public:
		Widget() = default;
		virtual ~Widget() = default;

	public:
		virtual void HandleEvent(Event& event) {};

		virtual void RenderGUI() = 0;

	};
}
