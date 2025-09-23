#include <spxpch.h>
#include "DetailsPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <typeindex>
#include <typeinfo>

#include <Reflection/Reflection.h>
#include <Component/Components.h>
#include <Scene/Scene.h>
#include <Asset/AssetManager.h>
#include <Asset/Sprite/SpriteAsset.h>
#include <Container/Array.h>
#include <Component/ComponentRegistry.h>
#include <ImGuiExtra.h>
#include <Base/Resources.h>
#include "Reflection/PropertyTree.h"
#include "PropertyViewer/PropertyViewer.h"


namespace Sphynx
{
#define LABEL(Label) LabelPrefix(Label)
	static std::string LabelPrefix(const char* label)
	{
		float width = ImGui::CalcItemWidth();

		float x = ImGui::GetCursorPosX();
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::SetNextItemWidth(-1);

		std::string labelStr = "##";
		labelStr += label;

		return labelStr;
	}

	template<typename T>
	void AddComponentType(Array<const Reflection::Class*>& list)
	{
		list.Add(&Reflection::GetClass<T>());
	}

	DetailsPanel::DetailsPanel(Widget* parent) :
		Panel("Details", parent),
		m_Context(),
		m_ComponetTypeToRemove(nullptr)
	{
	}

	DetailsPanel::DetailsPanel(const Actor& actor, Widget* parent) :
		Panel("Details", parent),
		m_Context(actor),
		m_ComponetTypeToRemove(nullptr)
	{
	}

	void DetailsPanel::SetContext(const Actor& context)
	{
		m_Context = context;
	}

	void DetailsPanel::RenderGUI()
	{
		if (!m_CanRender) return;
		if (!m_Context.IsValid()) return;

		Reflection::PropertyTree::Traverse(
			Reflection::GetClass<Actor>(), 
			&m_Context, 
			PropertyViewer{}
		);
	}

	void DetailsPanel::RenderComponent_ContextMenu(const char* label, const Reflection::Class& reflectionClass)
	{
		ImGui::SameLine();

		const char* label_end = ImGui::FindRenderedTextEnd(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, label_end, false);
		const ImVec2 padding = ImGui::GetStyle().FramePadding;
		const float frame_height = ImMax(ImMin(ImGui::GetCurrentWindow()->DC.CurrLineSize.y, ImGui::GetFontSize()), label_size.y);
		ImVec2 buttonSize{ frame_height, frame_height };

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - buttonSize.x - padding.x);

		ImGui::PushID(label);
		bool clicked = ImGui::ImageButton(label, (ImTextureID)Resources::ThreeDotsTexture->GetNativeTexture(), buttonSize);
		ImGui::PopID();

		if (clicked)
		{
			ImGui::OpenPopup(label);
		}

		if (ImGui::BeginPopup(label))
		{
			if (ImGui::MenuItem("Delete"))
			{
				m_ComponetTypeToRemove = &reflectionClass;
			}
			ImGui::EndPopup();
		}
	}
}