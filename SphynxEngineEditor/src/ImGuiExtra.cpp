#include "ImGuiExtra.h"


namespace ImGui
{
	bool SelectableItem(int& itemSelected, const int& numItem, const std::string& id, ImVec2 size, ImGuiSelectableFlags flags)
	{
		bool pressed = false;
		if (ImGui::Selectable(id.c_str(), itemSelected == numItem, flags, size))
		{
			itemSelected = numItem;
		}

		switch (flags)
		{
		case ImGuiSelectableFlags_None:
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				pressed = true;
			}
			break;
		case ImGuiSelectableFlags_AllowDoubleClick:
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				pressed = true;
			}
			break;
		default:
			break;
		}

		return pressed;
	}

	bool SelectableItemWithImageAndText(int& itemSelected, const int& numItem, const std::string& text, ImTextureID texture, ImVec2 size, ImVec4 tint, ImGuiSelectableFlags flags)
	{
		ImVec2 cursorPos = ImGui::GetCursorPos();

		std::string invisibleText = "##" + text;
		bool pressed = ImGui::SelectableItem(itemSelected, numItem, invisibleText, size, flags);

		ImGui::SetCursorPos(cursorPos);
		ImGui::SetItemAllowOverlap();

		ImGui::BeginGroup();

		ImGui::Image(texture, size, ImVec2(0, 0), ImVec2(1, 1), tint);

		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + size.x);
		ImGui::TextWrapped(text.c_str());
		ImGui::PopTextWrapPos();

		ImGui::EndGroup();

		return pressed;
	}
}