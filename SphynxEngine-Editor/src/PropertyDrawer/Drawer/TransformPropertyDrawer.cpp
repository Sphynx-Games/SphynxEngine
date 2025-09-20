#include "TransformPropertyDrawer.h"
#include "PropertyDrawer/PropertyDrawerManager.h"

#include <Reflection/Reflection.h>
#include <imgui.h>
#include <imgui_internal.h>


namespace Sphynx
{
	void TransformPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// TODO: this will be an Attribute in the future, for now we ShowInnerValues manually
		
		const Reflection::Class& cClass = Reflection::GetClass<Transform>();
		for (const Reflection::Property& prop : cClass)
		{
			IPropertyDrawer* drawer = PropertyDrawerManager::GetDrawer(prop.GetType());
			ImGui::PushID(prop.Name);
			drawer->Draw(prop, (std::byte*)data + prop.Offset);
			ImGui::PopID();
			if (ImGui::GetCurrentTable())
			{
				ImGui::TableNextRow();
			}
		}
	}
}
