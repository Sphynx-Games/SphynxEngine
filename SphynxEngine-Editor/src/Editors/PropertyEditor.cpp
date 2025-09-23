#include "spxpch.h"
#include "PropertyEditor.h"

#include "Panels/SceneOutlinerPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/DetailsPanel.h"
#include <Renderer/Framebuffer.h>
#include "PropertyViewer/PropertyViewer.h"

#include <imgui.h>
#include "Asset/Asset.h"
#include "Asset/AssetManager.h"
#include "imgui_internal.h"


namespace Sphynx
{
	PropertyEditor::PropertyEditor(const Reflection::Class& reflectionClass, void* object) :
		Editor("PropertyEditor"),
		m_ReflectionClass(reflectionClass),
		m_Object(object)
	{}

	PropertyEditor::~PropertyEditor() = default;

	void PropertyEditor::RenderGUI()
	{
		const ImGuiWindow* window = ImGui::FindWindowByName(GetName());
		const bool bIsWindowDocked = window != nullptr && window->DockNodeIsVisible;
		ImGuiWindowFlags flags = 0;
		flags |= !bIsWindowDocked * ImGuiWindowFlags_MenuBar;

		const bool visible = ImGui::Begin(GetName(), nullptr, flags);
		if (visible)
		{
			// Render Menu Bar
			if (ImGui::BeginMenuBar())
			{
				RenderMenuBar();
				ImGui::EndMenuBar();
			}

			// Render Toolbar (if valid)
			/*if (m_Toolbar != nullptr)
			{
				m_Toolbar->PreRenderGUI();
				m_Toolbar->RenderGUI();
				m_Toolbar->PostRenderGUI();
			}*/

			PropertyViewer propertyViewer{};
			Reflection::PropertyTree tree{ m_ReflectionClass, m_Object };
			tree.Traverse(propertyViewer);

			// Render child widgets
			for (Widget* widget : GetChildren())
			{
				widget->PreRenderGUI();
				widget->RenderGUI();
				widget->PostRenderGUI();
			}
		}
		ImGui::End();

		// This will append menu bar items into the parents menu bar
		// But only if the window has not shown them already
		if (visible && bIsWindowDocked && HasMenuBar())
		{
			if (ImGui::BeginMenuBar())
			{
				RenderMenuBar();
				ImGui::EndMenuBar();
			}
		}
	}

	void PropertyEditor::RenderMenuBar()
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();

			if (ImGui::MenuItem("Save", nullptr, nullptr, m_Object != nullptr))
			{
				SaveObject();
			}

			/*if (ImGui::MenuItem("Save As...", nullptr, nullptr, m_Object != nullptr))
			{
				auto path = Sphynx::FileDialog::Save();
				if (!path.empty())
				{
					SaveAsObject(path);
				}
			}*/

			ImGui::EndMenu();
		}
	}

	void PropertyEditor::SaveObject()
	{
		AssetType assetType{&m_ReflectionClass};
		SPX_CORE_ASSERT(AssetManager::IsAssetTypeRegistered(assetType), "Reflection class doesn't belong to a registered asset!!");

		AssetHandle handle = AssetManager::GetAssetHandleFromAddress(m_Object);
		SPX_CORE_ASSERT(handle != AssetHandle::Invalid, "Asset handle is invalid!!");

		// Save asset data in .spxasset
		AssetMetadata metadata = AssetManager::GetAssetMetadata(handle);
		AssetImporter::Save(metadata);
	}
}