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

	IMGUI_API bool SelectableItemWithImageAndTextInput(
		int& itemSelected,
		const int& numItem,
		const std::string& text,
		char* buffer,
		size_t bufferSize,
		ImTextureID texture,
		ImVec2 size = ImVec2{ 10.f, 10.f },
		ImVec4 tint = ImVec4{ 1.f, 1.f, 1.f, 1.f },
		ImGuiSelectableFlags flags = ImGuiSelectableFlags_None
	);

	IMGUI_API bool ButtonWithImageAndText(
		const std::string& text,
		ImTextureID texture,
		ImVec2 size = ImVec2{ 10.f, 10.f },
		float imageRatio = 0.75f,
		const ImVec2& uv0 = ImVec2(0, 0),
		const ImVec2& uv1 = ImVec2(1, 1),
		ImVec4 tint = ImVec4{ 1.f, 1.f, 1.f, 1.f }
	);

	IMGUI_API const char* CalcWordWrapPosition(const char* text, const char* text_end = nullptr, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

	//IMGUI_API bool SelectableInput(const char* str_id, bool selected, ImGuiSelectableFlags flags, char* buf, size_t buf_size);
}