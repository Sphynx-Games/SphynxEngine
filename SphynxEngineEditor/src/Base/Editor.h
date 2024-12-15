#pragma once

#include "Base/Widget.h"
#include <vector>
#include <string>


namespace Sphynx
{
	class Event;
	class Toolbar;

	class Editor : public Widget
	{
	public:
		Editor(const std::string& name);
		virtual ~Editor();

	public:
		void AddWidget(Widget* widget);
		void RemoveWidget(Widget* widget);
		void SetToolbar(Toolbar* toolbar);

		const std::string& GetName() const;

	protected:
		virtual void HandleEvent(Event& event) override;

		virtual void PreRenderGUI() override;
		virtual void RenderGUI() override;
		virtual void PostRenderGUI() override;

		// Menu bar
		virtual bool HasMenuBar() const { return false; }
		virtual void RenderMenuBar() {}

	protected:
		std::vector<Widget*> m_Widgets;
		Toolbar* m_Toolbar;
		std::string m_Name;
	};
}
