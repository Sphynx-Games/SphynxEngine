#pragma once

#include "Base/Panel.h"

#include <Scene/Actor.h>


namespace Sphynx
{
	class DetailsPanel : public Panel
	{
	public:
		DetailsPanel() = default;
		DetailsPanel(const Actor& context);

	public:
		void SetContext(const Actor& context);

	protected:
		virtual void RenderGUI() override;

	private:
		Actor m_Context;

	};
}
