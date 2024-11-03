#pragma once

#include <imgui.h>
#include <string>


namespace ImGui
{
	// select only one item
	IMGUI_API bool SelectableItem(
		int& itemSelected,
		const int& numItem,
		const std::string& id,
		ImVec2 size = ImVec2{10.f, 10.f},
		ImGuiSelectableFlags flags = ImGuiSelectableFlags_None
	);

	IMGUI_API bool SelectableItemWithImageAndText(
		int& itemSelected,
		const int& numItem,
		const std::string& text,
		ImTextureID texture,
		ImVec2 size = ImVec2{ 10.f, 10.f },
		ImVec4 tint = ImVec4{ 1.f, 1.f, 1.f, 1.f },
		ImGuiSelectableFlags flags = ImGuiSelectableFlags_None
	);
}