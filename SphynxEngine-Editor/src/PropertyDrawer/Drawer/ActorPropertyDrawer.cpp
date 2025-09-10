#include "ActorPropertyDrawer.h"

#include <Reflection/Reflection.h>
#include <Reflection/PropertyTree.h>
#include <PropertyViewer/PropertyViewer.h>

#include <Component/NameComponent.h>
#include <Component/UUIDComponent.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>


namespace Sphynx
{
	void ActorPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		Actor* actor = static_cast<Actor*>(data);

		// TODO: This does nothing for now
		bool isEnabled = false;
		ImGui::Checkbox("##IsEnabled", &isEnabled);
		ImGui::SameLine();
		ImGui::InputText("##Name", &actor->GetComponent<NameComponent>().Name);
		ImGui::Separator();
		// TODO: add component button

		Array<const Reflection::Class*> componentTypesToAdd;
		PropertyViewer propertyViewer{};

		// Show components
		for (const Reflection::Class* cClass : ComponentRegistry::GetComponents())
		{
			if (cClass == &Reflection::GetClass<NameComponent>()) continue;
			if (cClass == &Reflection::GetClass<UUIDComponent>()) continue;

			if (ComponentRegistry::InvokeHasComponent(*cClass, *actor, false))
			{
				void* component = ComponentRegistry::InvokeGetComponent(*cClass, *actor, false);
				Reflection::PropertyTree tree{ *cClass, component };
				tree.Traverse(propertyViewer);
			}
			else
			{
				componentTypesToAdd.Add(cClass);
			}
		}

		// Add component option
		if (ImGui::BeginPopupContextWindow("DetailsPanel_ContextMenu", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
		{
			if (ImGui::BeginMenu("New component"))
			{
				for (const auto& t : componentTypesToAdd)
				{
					std::string compName = t->Name;
					size_t pos = compName.find("::") + 2;
					compName = compName.substr(pos);
					if (ImGui::MenuItem(compName.c_str()))
					{
						ComponentRegistry::InvokeAddComponent(*t, *actor);
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		if (propertyViewer.InvisibleClass != nullptr)
		{
			ComponentRegistry::InvokeRemoveComponent(*propertyViewer.InvisibleClass, *actor);
		}
	}
}
