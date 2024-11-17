#include "ImGuiExtra.h"

#include <imgui_internal.h>
#include <cmath>


namespace ImGui
{
	static const char* CalcWordWrapNextLineStartA(const char* text, const char* text_end);


	bool SelectableItem(int& itemSelected, const int& numItem, const std::string& id, ImVec2 size, ImGuiSelectableFlags flags)
	{
		bool pressed = false;
		if (ImGui::Selectable(id.c_str(), itemSelected == numItem, flags | ImGuiSelectableFlags_AllowItemOverlap, size))
		{
			itemSelected = numItem;
		}

		switch (flags)
		{
		case ImGuiSelectableFlags_AllowDoubleClick:
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				pressed = true;
			}
			break;
		default:
			pressed = true;
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
		ImGui::SetNextItemAllowOverlap();

		ImGui::BeginGroup();

		ImGui::Image(texture, size, ImVec2(0, 0), ImVec2(1, 1), tint);

		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + size.x);
		ImGui::Text(text.c_str());
		ImGui::PopTextWrapPos();

		ImGui::EndGroup();

		return pressed;
	}

	bool SelectableItemWithImageAndTextInput(int& itemSelected, const int& numItem, const std::string& text, char* buffer, size_t bufferSize, ImTextureID texture, ImVec2 size, ImVec4 tint, ImGuiSelectableFlags flags)
	{
		ImVec2 cursorPos = ImGui::GetCursorPos();

		std::string invisibleText = "##" + text;
		ImGui::SelectableItem(itemSelected, numItem, invisibleText, size, flags);

		ImGui::SetCursorPos(cursorPos);
		ImGui::SetNextItemAllowOverlap();

		ImGui::BeginGroup();

		ImGui::Image(texture, size, ImVec2(0, 0), ImVec2(1, 1), tint);

		ImGui::PushItemWidth(size.x);

		if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			ImGui::SetKeyboardFocusHere(0);
		}

		strncpy(buffer, text.c_str(), text.size());
		bool renamed = ImGui::InputText("##rename", buffer, bufferSize, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::PopItemWidth();

		ImGui::EndGroup();

		return renamed;
	}

	bool ButtonWithImageAndText(const std::string& text, ImTextureID texture, ImVec2 size, float imageRatio, ImVec4 tint)
	{
		ImVec2 cursorPos = ImGui::GetCursorPos();
		ImVec2 padding = ImGui::GetStyle().FramePadding;
		cursorPos.x += padding.x;
		cursorPos.y += padding.y;
		ImVec2 availableSize = {size.x - 2.f * padding.x, size.y - 3.f * padding.y };

		int num = 1;
		std::string invisibleText = "##" + text;
		bool pressed = ImGui::Button(invisibleText.c_str(), size);

		const ImVec2 p0 = ImGui::GetItemRectMin();
		const ImVec2 p1 = ImGui::GetItemRectMax();
		ImGui::PushClipRect(p0, p1, true);

		ImGui::SetNextItemAllowOverlap();
		
		ImGui::BeginGroup();

		float imageSize = std::min(availableSize.x, availableSize.y * imageRatio);
		ImVec2 offset = { (availableSize.x - imageSize) / 2.0f, ((availableSize.y * imageRatio) - imageSize) / 2.0f};
		ImGui::SetCursorPos({cursorPos.x + offset.x, cursorPos.y + offset.y });

		ImGui::Image(texture, { imageSize, imageSize }, ImVec2(0, 0), ImVec2(1, 1), tint);

		ImGui::SetCursorPos({cursorPos.x, cursorPos.y + (availableSize.y * imageRatio) + padding.y });
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availableSize.x);
		const char* textBegin = text.c_str();
		const char* textEnd = text.c_str() + text.size();
		while (textBegin != textEnd)
		{
			const char* nextText = CalcWordWrapPosition(textBegin, textEnd, false, availableSize.x);

			float textWidth = ImGui::CalcTextSize(textBegin, nextText, false, availableSize.x).x;
			float textIndent = (availableSize.x - textWidth) * 0.5f;
			if (textIndent < 0.0f)
			{
				textIndent = 0.0f;
			}

			ImGui::SetCursorPosX(cursorPos.x + textIndent);
			ImGui::TextUnformatted(textBegin, nextText);

			textBegin = CalcWordWrapNextLineStartA(nextText, textEnd);
		}
		ImGui::PopTextWrapPos();

		ImGui::EndGroup();

		ImGui::PopClipRect();

		return pressed;
	}

	const char* CalcWordWrapPosition(const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width)
	{
		ImGuiContext& g = *GImGui;

		const char* text_display_end;
		if (hide_text_after_double_hash)
		{
			text_display_end = FindRenderedTextEnd(text, text_end);  // Hide anything after a '##' string
		}
		else
		{
			text_display_end = text_end;
		}

		ImFont* font = g.Font;
		const float font_size = g.FontSize;
		if (text == text_display_end)
		{
			return text_display_end;
		}
		return font->CalcWordWrapPositionA(font_size / font->FontSize, text, text_display_end, wrap_width);
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


	static const char* CalcWordWrapNextLineStartA(const char* text, const char* text_end)
	{
		while (text < text_end && ImCharIsBlankA(*text))
			text++;
		if (*text == '\n')
			text++;
		return text;
	}
}