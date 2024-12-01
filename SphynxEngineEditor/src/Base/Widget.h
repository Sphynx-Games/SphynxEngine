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

		virtual void PreRenderGUI() {};
		virtual void RenderGUI() = 0;
		virtual void PostRenderGUI() {};

	};
}
