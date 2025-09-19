#include "AssetHandlePropertyDrawer.h"

#include <Asset/AssetManager.h>
#include <imgui.h>


namespace Sphynx
{
	void AssetHandlePropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		AssetHandle* assetHandle = static_cast<AssetHandle*>(data);
		std::string selectedPath = *assetHandle != AssetHandle::Invalid
			? AssetManager::GetAssetMetadata(*assetHandle).Path.stem().string()
			: "";

		if (ImGui::BeginCombo("##", selectedPath.c_str()))
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
