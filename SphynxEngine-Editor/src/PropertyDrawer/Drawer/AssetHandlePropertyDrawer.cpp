#include "AssetHandlePropertyDrawer.h"

#include <Asset/AssetManager.h>
#include <imgui.h>


namespace Sphynx
{
	void AssetHandlePropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		AssetHandle* assetHandle = static_cast<AssetHandle*>(data);
		const AssetMetadata& selectedMetadata = AssetManager::GetAssetMetadata(*assetHandle);

		IPropertyDrawer::DrawDefaultLabel(property);

		if (ImGui::BeginCombo("##", selectedMetadata.Path.stem().string().c_str()))
		{
			// TODO: filter by attribute if possible
			const auto& registry = AssetManager::GetRegistry();
			for (const auto& [handle, metadata] : AssetManager::GetRegistry())
			{
				if (ImGui::Selectable(metadata.Path.stem().string().c_str(), handle == *assetHandle))
				{
					*assetHandle = metadata.Handle;
				}
			}
			ImGui::EndCombo();
		}
	}
}
