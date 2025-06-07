#include "PropertyViewer.h"
#include <Reflection/Property.h>
#include "Reflection/Class.h"
#include "imgui.h"
#include "PropertyDrawer/PropertyDrawerManager.h"
#include "PropertyDrawer/PropertyDrawer.h"
#include "Asset/AssetMetadata.h"


namespace Sphynx
{
#define LABEL(Label) LabelPrefix(Label).c_str()
	static std::string LabelPrefix(const char* label)
	{
		std::string labelStr = "##";
		labelStr += label;

		return labelStr;
	}

	void PropertyViewer::Visit(const Reflection::Property* property, bool& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

		bool buffer = data;
		if (ImGui::Checkbox(LABEL(property->Name), &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, char& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

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

		IPropertyDrawer::DrawDefaultLabel(*property);

		ImGui::LabelText(LABEL(property->Name), "Not implemented");
	}

	void PropertyViewer::Visit(const Reflection::Property* property, wchar_t& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

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

		IPropertyDrawer::DrawDefaultLabel(*property);

		short buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_S16, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, int& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

		int buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_S32, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, long& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

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

		IPropertyDrawer::DrawDefaultLabel(*property);

		long long buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_S64, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, float& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

		float buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_Float, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, double& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

		double buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_Double, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, long double& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

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

		IPropertyDrawer::DrawDefaultLabel(*property);

		unsigned char buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_U8, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, unsigned short& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

		unsigned short buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_U16, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, unsigned int& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

		unsigned int buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_U32, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, unsigned long& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

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

		IPropertyDrawer::DrawDefaultLabel(*property);

		unsigned long long buffer = data;
		if (ImGui::DragScalar(LABEL(property->Name), ImGuiDataType_U64, &buffer))
		{
			data = buffer;
		}
	}

	void PropertyViewer::Visit(const Reflection::Property* property, ::std::string& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

		ImGui::LabelText(LABEL(property->Name), "Not implemented");
	}

	void PropertyViewer::Visit(const Reflection::Property* property, ::std::wstring& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

		ImGui::LabelText(LABEL(property->Name), "Not implemented");
	}

	void PropertyViewer::Visit(const Reflection::Property* property, ::std::filesystem::path& data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

		ImGui::LabelText(LABEL(property->Name), "Not implemented");
	}

	void PropertyViewer::VisitEnum(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		IPropertyDrawer::DrawDefaultLabel(*property);

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
			propertyDrawer->Draw(*property, data);
			return false;
		}

		const char* label = std::strrchr(property->Name, ':');
		if (label == nullptr) label = property->Name;
		else label += 1;

		const bool result = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen);
		ImGui::Indent();
		return result;
	}

	bool PropertyViewer::VisitClass(const Reflection::Property* property, void* data, const Reflection::AssociativeCollection& collection)
	{
		//if (property->IsPointer()) return false;

		ImGui::LabelText(LABEL(property->Name), "Not implemented");
		return false;
	}

	bool PropertyViewer::VisitClass(const Reflection::Property* property, void* data, const Reflection::IndexedCollection& collection)
	{
		//if (property->IsPointer()) return false;

		ImGui::LabelText(LABEL(property->Name), "Not implemented");
		return false;
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

		ImGui::PopID();
	}
}