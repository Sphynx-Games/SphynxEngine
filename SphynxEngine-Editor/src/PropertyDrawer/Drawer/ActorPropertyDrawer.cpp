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

		// Show components
		for (const Reflection::Class* cClass : ComponentRegistry::GetComponents())
		{
			if (cClass == &Reflection::GetClass<NameComponent>()) continue;
			if (cClass == &Reflection::GetClass<UUIDComponent>()) continue;

			if (ComponentRegistry::InvokeHasComponent(*cClass, *actor, false))
			{
				void* component = ComponentRegistry::InvokeGetComponent(*cClass, *actor, false);
				Reflection::PropertyTree tree{ *cClass, component };
				tree.Traverse(PropertyViewer{});
			}
		}
	}
}
