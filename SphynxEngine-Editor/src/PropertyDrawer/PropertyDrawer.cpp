#include "PropertyDrawer.h"
#include <xutility>
#include <imgui.h>
#include <string>


namespace Sphynx
{
	std::vector<IPropertyDrawer*>* IPropertyDrawer::s_Registry = nullptr;

	void IPropertyDrawer::DrawDefaultLabel(const Reflection::Property& property)
	{
		const char* label = std::strrchr(property.Name, ':');
		if (label == nullptr) label = property.Name;
		else label += 1;
		DrawDefaultLabel(label);
	}

	void IPropertyDrawer::DrawDefaultLabel(const char* label)
	{
		float width = ImGui::CalcItemWidth();

		float x = ImGui::GetCursorPosX();
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::SetNextItemWidth(-1);
	}

	void IPropertyDrawer::EnsureInitialized()
	{
		if (s_Registry == nullptr)
		{
			static std::vector<IPropertyDrawer*> sRegistry{};
			IPropertyDrawer::s_Registry = &sRegistry;
		}
	}

	void IPropertyDrawer::RegisterImpl::Register(IPropertyDrawer* drawer)
	{
		EnsureInitialized();
		s_Registry->push_back(drawer);
	}

	void IPropertyDrawer::RegisterImpl::Unregister(IPropertyDrawer* drawer)
	{
		EnsureInitialized();
		s_Registry->erase(std::find(s_Registry->begin(), s_Registry->end(), drawer));
	}
}
