#pragma once

#include "Base/Panel.h"

#include <Scene/Actor.h>
#include <imgui.h>


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
		template<typename T>
		void RenderComponent_ContextMenu(const char* id)
		{
			if (ImGui::BeginPopup(id))
			{
				if (ImGui::MenuItem("Delete"))
				{
					m_Context.RemoveComponent<T>();
				}
				ImGui::EndPopup();
			}
		}

	private:
		Actor m_Context;

	};
}
