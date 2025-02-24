#pragma once

#include "Base/Widget.h"


namespace Sphynx
{
	class Panel : public Widget
	{
	public:
		using Widget::Widget;

	protected:
		virtual void PreRenderGUI() override;
		virtual void RenderGUI() override;
		virtual void PostRenderGUI() override;

	protected:
		bool m_CanRender;

	};
}