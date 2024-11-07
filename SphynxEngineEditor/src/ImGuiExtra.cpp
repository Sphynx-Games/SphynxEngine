#include "ImGuiExtra.h"

#include <imgui_internal.h>


namespace ImGui
{
	bool SelectableItem(int& itemSelected, const int& numItem, const std::string& id, ImVec2 size, ImGuiSelectableFlags flags)
	{
		bool pressed = false;
		if (ImGui::Selectable(id.c_str(), itemSelected == numItem, flags | ImGuiSelectableFlags_AllowItemOverlap, size))
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

	bool SelectableItemWithImageAndTextInput(int& itemSelected, const int& numItem, const std::string& text, char* buffer, ImTextureID texture, ImVec2 size, ImVec4 tint, ImGuiSelectableFlags flags)
	{
		ImVec2 cursorPos = ImGui::GetCursorPos();

		std::string invisibleText = "##" + text;
		ImGui::SelectableItem(itemSelected, numItem, invisibleText, size, flags);

		ImGui::SetCursorPos(cursorPos);
		ImGui::SetItemAllowOverlap();

		ImGui::BeginGroup();

		ImGui::Image(texture, size, ImVec2(0, 0), ImVec2(1, 1), tint);

		ImGui::PushItemWidth(size.x);

		if (/*ImGui::IsAnyItemFocused() &&*/ !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			ImGui::SetKeyboardFocusHere(0);

		strncpy(buffer, text.c_str(), sizeof(buffer));
		bool renamed = ImGui::InputText("##rename", buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue /* | ImGuiInputTextFlags_CallbackResize*/);

		/*ImGuiInputTextState& edit_state = ImGui::GetCurrentContext()->InputTextState;
		edit_state.TextA.Size = 1024;
		edit_state.InitialTextA.Size = 1024;*/

		ImGui::PopItemWidth();

		ImGui::EndGroup();

		return renamed;
	}

	/*bool SelectableInput(const char* str_id, bool selected, ImGuiSelectableFlags flags, char* buf, size_t buf_size)
	{
		using namespace ImGui;
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImVec2 pos = window->DC.CursorPos;

		PushID(str_id);
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(g.Style.ItemSpacing.x, g.Style.FramePadding.y * 2.0f));
		bool ret = Selectable("##Selectable", selected, flags | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap);
		PopStyleVar();

		ImGuiID id = window->GetID("##Input");
		bool temp_input_is_active = TempInputIsActive(id);
		bool temp_input_start = ret ? IsMouseDoubleClicked(0) : false;
		if (temp_input_is_active || temp_input_start)
		{
			ret = TempInputText(g.LastItemData.Rect, id, "##Input", buf, (int)buf_size, ImGuiInputTextFlags_None);
			KeepAliveID(id);
		}
		else
		{
			window->DrawList->AddText(pos, GetColorU32(ImGuiCol_Text), buf);
		}

		PopID();
		return ret;
	}*/
}