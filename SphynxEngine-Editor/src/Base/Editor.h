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
		Editor(const char* name, Widget* parent = nullptr);
		virtual ~Editor();

	public:
		void SetToolbar(Toolbar* toolbar);

	protected:
		virtual void PreRenderGUI() override {}
		virtual void RenderGUI() override;
		virtual void PostRenderGUI() override {}

		// Menu bar
		virtual bool HasMenuBar() const { return false; }
		virtual void RenderMenuBar() {}

	protected:
		Toolbar* m_Toolbar;
	};
}
