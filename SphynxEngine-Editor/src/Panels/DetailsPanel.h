#pragma once

#include "Base/Panel.h"

#include <Scene/Actor.h>
#include <Container/Array.h>


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
		void RenderComponent_ContextMenu(const char* label, const Reflection::Class& reflectionClass);

	private:
		Actor m_Context;
		const Reflection::Class* m_ComponetTypeToRemove;

	};
}
