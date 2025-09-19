#include "PropertyViewer.h"
#include <Reflection/Property.h>
#include "Reflection/Class.h"
#include "imgui.h"
#include "PropertyDrawer/PropertyDrawerManager.h"
#include "PropertyDrawer/PropertyDrawer.h"
#include "Asset/AssetMetadata.h"
#include "imgui_internal.h"


namespace Sphynx
{
#define LABEL(Label) LabelPrefix(Label).c_str()
	static std::string LabelPrefix(const char* label)
	{
		std::string labelStr = "##";
		labelStr += label;

		return labelStr;
	}

	PropertyViewer::PropertyViewer() :
		m_IsTableSetup(false),
		m_IsIndexedCollectionViewerOpened(false)
	{
	}

	PropertyViewer::~PropertyViewer()
	{
	}

	void PropertyViewer::SetupTable()
	{
		SPX_CORE_ASSERT(m_IsTableSetup == false, "You should call FinishTable() before setting it up again");

		ImGui::PushOverrideID(ImHashStr("PropertiesTable"));
		m_IsTableSetup = ImGui::BeginTable("##properties", 2, ImGuiTableFlags_Resizable);
		if (m_IsTableSetup)
		{
			ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 2.0f);
		}
		else
		{
			ImGui::PopID();
		}
	}

	void PropertyViewer::FinishTable()
	{
		SPX_CORE_ASSERT(m_IsTableSetup == true, "You should call SetupTable() before finishing it up again");
		ImGui::EndTable();
		ImGui::PopID();
		m_IsTableSetup = false;
	}

	void PropertyViewer::Visit(const Reflection::Property* property, bool& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		bool buffer = data;
		if (ImGui::Checkbox(LABEL(property->Name), &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, char& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		const size_t size = 1;
		char buffer = data;
		if (ImGui::InputText(LABEL(property->Name), &buffer, size))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, signed char& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		ImGui::LabelText(LABEL(property->Name), "Not implemented");
	}

	void PropertyViewer::Visit(const Reflection::Property* property, wchar_t& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		const size_t size = 1;
		char buffer = static_cast<char>(data);
		if (ImGui::InputText(LABEL(property->Name), &buffer, size))
		{
			data = static_cast<wchar_t>(buffer);
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, short& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		short buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_S16, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, int& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		int buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_S32, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, long& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		if constexpr (sizeof(long) == 32)
		{
			Visit(property, (int32_t&)data);
		}
		else
		{
			Visit(property, (int64_t&)data);
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, long long& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		long long buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_S64, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, float& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		float buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_Float, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, double& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		double buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_Double, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, long double& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		// FIXME: no support for long double
		double buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_Double, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, unsigned char& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		unsigned char buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_U8, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, unsigned short& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		unsigned short buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_U16, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, unsigned int& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		unsigned int buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_U32, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, unsigned long& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		if constexpr (sizeof(unsigned long) == 32)
		{
			Visit(property, (uint32_t&)data);
		}
		else
		{
			Visit(property, (uint64_t&)data);
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, unsigned long long& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		unsigned long long buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_U64, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, ::std::string& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		ImGui::LabelText(LABEL(property->Name), "Not implemented");
	}

	void PropertyViewer::Visit(const Reflection::Property* property, ::std::wstring& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		ImGui::LabelText(LABEL(property->Name), "Not implemented");
	}

	void PropertyViewer::Visit(const Reflection::Property* property, ::std::filesystem::path& data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		ImGui::LabelText(LABEL(property->Name), "Not implemented");
	}

	void PropertyViewer::VisitEnum(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		if(m_IsTableSetup) ImGui::TableNextColumn();
		IPropertyDrawer::DrawDefaultLabel(*property);

		if (m_IsTableSetup) ImGui::TableNextColumn();
		const Reflection::Enum& rEnum = static_cast<const Reflection::Enum&>(property->Type);
		const char* currentValue = rEnum.GetName((const void*)data).c_str();
		if (ImGui::BeginCombo(LABEL(property->Name), currentValue))
		{
			for (const Reflection::Enum::Entry& entry : rEnum)
			{
				const char* optionName = entry.Name.c_str();
				if (ImGui::Selectable(optionName, false))
				{
					rEnum.SetValue(data, entry.Value);
				}
			}
			ImGui::EndCombo();
		}
	}

	bool PropertyViewer::VisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return false;

		IPropertyDrawer* propertyDrawer = PropertyDrawerManager::GetDrawer(property->Type);
		if (propertyDrawer)
		{
			if (m_IsTableSetup) ImGui::TableNextColumn();
			propertyDrawer->Draw(*property, data);
			return false;
		}

		const char* label = std::strrchr(property->Name, ':');
		if (label == nullptr) label = property->Name;
		else label += 1;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		bool visible = true;
		const bool result = ImGui::CollapsingHeader(label, &visible, flags);

		// If root property then do begin table
		if (property->Name == property->Type.Name && result && !m_IsTableSetup)
		{
			SetupTable();
		}

		ImGui::Indent();

		if (!visible)
		{
			// TODO: send command to delete this property when command system is implemented
			InvisibleClass = &static_cast<const Reflection::Class&>(property->Type);
		}

		return result;
	}

	bool PropertyViewer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		//if (property->IsPointer()) return false;

		ImGui::LabelText(LABEL(property->Name), "Not implemented");
		return false;
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, bool& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, char& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, signed char& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, wchar_t& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, short& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, int& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, long& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, long long& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, float& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, double& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, long double& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, unsigned char& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, unsigned short& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, unsigned int& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, unsigned long& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, unsigned long long& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, ::std::string& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, ::std::wstring& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	void PropertyViewer::OnBeforeVisit(const Reflection::Property* property, ::std::filesystem::path& data)
	{
		if (property->IsPointer()) return;

		if (m_IsTableSetup)
		{
			ImGui::TableNextRow();
		}
	}

	bool PropertyViewer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (property->IsPointer()) return false;

		IPropertyDrawer* propertyDrawer = PropertyDrawerManager::GetDrawer(property->Type);
		if (propertyDrawer)
		{
			propertyDrawer->Draw(*property, data);
			return false;
		}

		const size_t count = collection.GetSize(data);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowOverlap;

		// This is done to offset the arrow so the label is align with other labels
		ImGuiContext& g = *ImGui::GetCurrentContext();
		const ImGuiStyle& style = g.Style;
		const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
		const float padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding.x : style.FramePadding.x;
		const float text_offset_x = g.FontSize + (display_frame ? padding * 3 : padding * 2);   // Collapsing arrow width + Spacing

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));

		if (m_IsTableSetup)
		{
			ImGui::TableNextColumn();
			flags |= ImGuiTreeNodeFlags_SpanAllColumns;
		}

		const float prevCursorPosX = ImGui::GetCursorPosX();
		ImGui::SetCursorPosX(prevCursorPosX - text_offset_x);
		m_IsIndexedCollectionViewerOpened = ImGui::TreeNodeEx(property->Name, flags);
		ImGui::SetCursorPosX(prevCursorPosX);

		ImGui::PopStyleColor(3);

		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		float buttonSize = ImGui::GetFrameHeight();
		float totalButtonWidth = buttonSize * 2 + spacing; // 2 buttons for now

		if (m_IsTableSetup) ImGui::TableNextColumn();
		else ImGui::SameLine();

		float x = ImGui::GetContentRegionMax().x - totalButtonWidth;
		ImGui::SetCursorPosX(x);

		if (ImGui::SmallButton("+"))
		{
			collection.Add(data);
		}

		ImGui::SameLine();

		if (count == 0) ImGui::BeginDisabled();
		if (ImGui::SmallButton("-"))
		{
			collection.RemoveAt(data, collection.GetSize(data) - 1);
		}
		if (count == 0) ImGui::EndDisabled();

		ImGui::Indent();

		return m_IsIndexedCollectionViewerOpened;
	}

	void PropertyViewer::OnBeforeVisitEnum(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		ImGui::PushID(property->Name);
	}

	void PropertyViewer::OnAfterVisitEnum(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		ImGui::PopID();
	}

	void PropertyViewer::OnBeforeVisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		ImGui::PushID(property->Name);
	}

	void PropertyViewer::OnAfterVisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		auto* propertyDrawer = PropertyDrawerManager::GetDrawer(property->Type);
		if (propertyDrawer == nullptr)
		{
			ImGui::Unindent();
		}

		// If root property then do begin table
		if (property->Name == property->Type.Name)
		{
			if (m_IsTableSetup)
			{
				FinishTable();
			}
		}

		ImGui::PopID();
	}

	void PropertyViewer::OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (property->IsPointer()) return;

		ImGui::PushID(property->Name);
	}

	void PropertyViewer::OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (property->IsPointer()) return;

		auto* propertyDrawer = PropertyDrawerManager::GetDrawer(property->Type);
		if (propertyDrawer == nullptr)
		{
			ImGui::Unindent();
		}

		if (m_IsIndexedCollectionViewerOpened)
		{
			ImGui::TreePop();
			m_IsIndexedCollectionViewerOpened = false;
		}

		ImGui::PopID();
	}
}