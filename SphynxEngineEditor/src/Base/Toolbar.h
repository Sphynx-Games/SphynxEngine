#pragma once

#include "Base/Widget.h"


namespace Sphynx
{
	class Toolbar : public Widget
	{
	public:
		Toolbar();
		~Toolbar();

		void AddWidget(Widget* widget);
		void RemoveWidget(Widget* widget);

	public:
		virtual void HandleEvent(Event& event) override;
		virtual void RenderGUI() override;

	protected:
		std::vector<Widget*> m_Widgets;
	};
}