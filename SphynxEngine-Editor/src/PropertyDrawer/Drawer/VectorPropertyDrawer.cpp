#include "VectorPropertyDrawer.h"

#include "imgui.h"


namespace Sphynx
{
	void Vector2iPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_S32, data, 2, 1.0f);
	}

	void Vector2uPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_U32, data, 2, 1.0f);
	}

	void Vector2fPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Float, data, 2, 0.1f);
	}

	void Vector2dPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Double, data, 2, 0.1f);
	}

	void Vector3iPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_S32, data, 3, 1.0f);
	}

	void Vector3uPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_U32, data, 3, 1.0f);
	}

	void Vector3fPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Float, data, 3, 0.1f);
	}

	void Vector3dPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Double, data, 3, 0.1f);
	}

	void Vector4iPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_S32, data, 4, 1.0f);
	}

	void Vector4uPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_U32, data, 4, 1.0f);
	}

	void Vector4fPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Float, data, 4, 0.1f);
	}

	void Vector4dPropertyDrawer::DrawWidget(const Reflection::Property& property, void* data)
	{
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Double, data, 4, 0.1f);
	}
}
