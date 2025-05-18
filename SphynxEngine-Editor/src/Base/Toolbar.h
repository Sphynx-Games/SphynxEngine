#pragma once

#include "Base/Widget.h"


namespace Sphynx
{
	class Toolbar : public Widget
	{
	public:
		Toolbar(Widget* parent = nullptr);
		~Toolbar();

	public:
		virtual void RenderGUI() override;

	};
}