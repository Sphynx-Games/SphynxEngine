#pragma once

#include "Base/Panel.h"

#include <Scene/Actor.h>


namespace Sphynx
{
	class DetailsPanel : public Panel
	{
	public:
		DetailsPanel(Widget* parent = nullptr);
		DetailsPanel(const Actor& context, Widget* parent = nullptr);

	public:
		void SetContext(const Actor& context);

	protected:
		virtual void RenderGUI() override;

	private:
		Actor m_Context;

	};
}
