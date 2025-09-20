#include "AssetHandlePropertyDrawer.h"

#include <Asset/AssetManager.h>
#include <imgui.h>
#include "Attribute/AssetHandleType.h"


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
			// TODO: maybe a better way to filter out instead of looping through all assets
			const EditorAttribute::AssetHandleType* assetHandleType = property.GetAttribute<EditorAttribute::AssetHandleType>();
			for (const auto& [handle, metadata] : AssetManager::GetRegistry())
			{
				if (assetHandleType != nullptr && metadata.Type != assetHandleType->Type) continue;

				if (ImGui::Selectable(metadata.Path.stem().string().c_str(), handle == *assetHandle))
				{
					*assetHandle = metadata.Handle;
				}
			}
			ImGui::EndCombo();
		}
	}
}
