#include "VectorPropertyDrawer.h"

#include "imgui.h"


namespace Sphynx
{
	void Vector2iPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_S32, data, 2, 1.0f);
	}

	void Vector2uPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_U32, data, 2, 1.0f);
	}

	void Vector2fPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Float, data, 2, 0.1f);
	}

	void Vector2dPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Double, data, 2, 0.1f);
	}

	void Vector3iPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_S32, data, 3, 1.0f);
	}

	void Vector3uPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_U32, data, 3, 1.0f);
	}

	void Vector3fPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Float, data, 3, 0.1f);
	}

	void Vector3dPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Double, data, 3, 0.1f);
	}

	void Vector4iPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_S32, data, 4, 1.0f);
	}

	void Vector4uPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_U32, data, 4, 1.0f);
	}

	void Vector4fPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Float, data, 4, 0.1f);
	}

	void Vector4dPropertyDrawer::Draw(const Reflection::Property& property, void* data)
	{
		// Draw label
		IPropertyDrawer::DrawDefaultLabel(property);

		// Draw value
		std::string label{ "##" };
		label += property.Name;
		ImGui::DragScalarN(label.c_str(), ImGuiDataType_Double, data, 4, 0.1f);
	}
}
