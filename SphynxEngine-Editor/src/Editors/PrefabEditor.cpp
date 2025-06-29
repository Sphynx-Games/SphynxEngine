#include "spxpch.h"
#include "PrefabEditor.h"

#include "Panels/SceneOutlinerPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/DetailsPanel.h"
#include <Renderer/Framebuffer.h>
#include <Asset/AssetManager.h>
#include <Asset/Prefab/PrefabAsset.h>
#include <Component/ComponentRegistry.h>
#include <Component/UUIDComponent.h>

#include <imgui.h>
#include "Scene/SceneRenderer.h"
#include "Events/Event.h"
#include "Core/Time.h"


namespace Sphynx
{
	PrefabEditor::PrefabEditor(Prefab* prefab) :
		Editor("PrefabEditor"),
		m_SceneOutlinerPanel(new SceneOutlinerPanel()),
		m_ViewportPanel(new ViewportPanel()),
		m_DetailsPanel(new DetailsPanel()),
		m_Framebuffer(nullptr),
		m_Prefab(prefab),
		m_Scene(),
		m_CameraController()
	{
		AddWidget(m_SceneOutlinerPanel);
		AddWidget(m_ViewportPanel);
		AddWidget(m_DetailsPanel);

		m_Framebuffer = Framebuffer::Create({ 1920, 1080, 0, { FramebufferTextureFormat::RGBA8 } });
		m_ViewportPanel->SetFramebuffer(m_Framebuffer);
		m_ViewportPanel->SetIndex(0);

		Actor& actor = m_Scene.CreateActor();
		Actor::CloneComponents(*m_Prefab, actor);

		m_SceneOutlinerPanel->SetContext(&m_Scene);
		m_DetailsPanel->SetContext(actor);
	}

	PrefabEditor::~PrefabEditor()
	{
		if (m_Framebuffer != nullptr)
		{
			delete m_Framebuffer;
			m_Framebuffer = nullptr;
		}
	}

	void PrefabEditor::RenderGUI()
	{
		m_Framebuffer->Bind();
		const float aspectRatio = (float)m_Framebuffer->GetSpecification().Width / (float)m_Framebuffer->GetSpecification().Height;
		m_CameraController.SetAspectRatio(aspectRatio);
		SceneRenderer::Render(m_Scene, &m_CameraController.GetCamera());
		m_Framebuffer->Unbind();

		const float deltaTime = Time::GetScaledDeltaTime();
		m_CameraController.Update(deltaTime);

		Editor::RenderGUI();
	}

	void PrefabEditor::HandleEvent(Event& event)
	{
		Editor::HandleEvent(event);
		if (event.IsHandled()) return;

		m_CameraController.HandleEvent(event);
	}

	void PrefabEditor::RenderMenuBar()
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();

			if (ImGui::MenuItem("Save", nullptr, nullptr, m_Prefab != nullptr))
			{
				SavePrefab();
			}

			/*if (ImGui::MenuItem("Save As...", nullptr, nullptr, m_Prefab != nullptr))
			{
				auto path = Sphynx::FileDialog::Save();
				if (!path.empty())
				{
					SaveAsScene(path);
				}
			}*/

			ImGui::EndMenu();
		}
	}

	void PrefabEditor::SavePrefab()
	{
		AssetHandle handle = AssetManager::GetAssetHandleFromAddress(m_Prefab);
		SPX_CORE_ASSERT(handle != AssetHandle::Invalid, "Asset handle is invalid!!");
		
		const Array<Actor>& actors = m_Scene.GetActors();
		SPX_CORE_ASSERT(actors.Size() == 1, "Scene for PrefabEditor must have only one actor!!");

		// Remove all components from prefab (except UUID)
		for (const Reflection::Class* componentClass : ComponentRegistry::GetComponents())
		{
			if (componentClass == &Reflection::GetClass<UUIDComponent>()) continue;
			if (void* component = ComponentRegistry::InvokeGetComponent(*componentClass, *m_Prefab, false))
			{
				ComponentRegistry::InvokeRemoveComponent(*componentClass, *m_Prefab);
			}
		}

		// Clone component from actor to prefab
		Actor::CloneComponents(actors[0], *m_Prefab);

		// Save prefab data in .spxasset
		AssetMetadata metadata = AssetManager::GetAssetMetadata(handle);
		AssetImporter::Save(metadata);
	}
}