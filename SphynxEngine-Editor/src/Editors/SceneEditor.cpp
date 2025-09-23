#include "spxpch.h"
#include "SceneEditor.h"

#include "Base/Resources.h"
#include "EditorLayer.h"
#include "Panels/SceneToolbar.h"
#include "Panels/SceneOutlinerPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/DetailsPanel.h"

#include "Widgets/ButtonWidget.h"

#include "Core/Time.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Framebuffer.h"
#include "Scene/SceneRenderer.h"

#include "Asset/Scene/SceneAsset.h"
#include "ProjectEditor.h"
#include "PrefabEditor.h"
#include "PropertyEditor.h"
#include "Scene/EditorSceneSerializer.h"
#include "Scene/EditorSceneDeserializer.h"

// TODO: remove
#include <Sphynx.h>
#include "Asset/AssetManager.h"
#include <Renderer/Sprite.h>

#include <imgui.h>
#include "Dialogs/FileDialog.h"
#include "imgui_internal.h"

#include <Asset/Prefab/PrefabAsset.h>
#include <Serialization/Prefab/PrefabSerializer.h>
#include <Serialization/Prefab/PrefabDeserializer.h>


namespace Sphynx
{
	SceneEditor::SceneEditor(EditorLayer* editorLayer) :
		Editor("SceneEditor"),
		m_OpenedProjectHandle(),
		m_EditPrefabHandle(),
		m_EditGenericAssetHandle(),
		m_EditorLayer(editorLayer),
		m_SceneToolbar(new SceneToolbar()),
		m_SceneOutlinerPanel(new SceneOutlinerPanel()),
		m_ContentBrowserPanel(new ContentBrowserPanel()),
		m_ViewportPanel(new ViewportPanel()),
		m_DetailsPanel(new DetailsPanel()),
		m_Framebuffer(nullptr),
		m_CameraController(),
		m_LastOpenedScenePath(),
		m_SceneToEdit(),
		m_SceneToPlay(),
		m_ActiveScene(nullptr),
		m_SceneState(PlaybackState::STOPPED),
		m_Ejected(false),
		m_SceneNameBuffer()
	{
		m_OpenedProjectHandle = ProjectEditor::OnProjectOpened.Bind(this, &SceneEditor::ManageProjectOpened);
		m_EditPrefabHandle = m_ContentBrowserPanel->OnPrefabEdit.Bind(this, &SceneEditor::OpenPrefabEditor);
		m_EditGenericAssetHandle = m_ContentBrowserPanel->OnGenericAssetEdit.Bind(this, &SceneEditor::OpenPropertyEditor);

		SetToolbar(m_SceneToolbar);
		AddWidget(m_SceneOutlinerPanel);
		AddWidget(m_ContentBrowserPanel);
		AddWidget(m_ViewportPanel);
		AddWidget(m_DetailsPanel);

		m_Framebuffer = Framebuffer::Create({ 1920, 1080, 0, { FramebufferTextureFormat::RGBA8 } });
		m_ViewportPanel->SetFramebuffer(m_Framebuffer);
		m_ViewportPanel->SetIndex(0);

		m_SceneToolbar->SetPlaybackState(m_SceneState);
		m_SceneOutlinerPanel->SetContext(m_ActiveScene);

		// save button
		m_SceneToolbar->GetSaveButton()->OnClick.Bind(this, &SceneEditor::SaveScene);

		// play and pause button
		m_SceneToolbar->GetPlayAndPauseButton()->OnClick.Bind([&]() {
			switch (m_SceneState)
			{
			case PlaybackState::PLAYING:
				PauseScene();
				break;
			case PlaybackState::PAUSED:
				ResumeScene();
				break;
			case PlaybackState::STOPPED:
				PlayScene();
				break;
			default:
				break;
			}
			});

		// stop button
		m_SceneToolbar->GetStopButton()->OnClick.Bind(this, &SceneEditor::StopScene);

		// eject button
		m_SceneToolbar->GetEjectButton()->OnClick.Bind([&]() {
			if (m_Ejected)
			{
				DeactivateEject();
			}
			else
			{
				ActivateEject();
			}
			});
	}

	SceneEditor::~SceneEditor()
	{
		ProjectEditor::OnProjectOpened.Unbind(m_OpenedProjectHandle);
		m_ContentBrowserPanel->OnPrefabEdit.Unbind(m_EditPrefabHandle);

		if (m_ActiveScene != nullptr)
		{
			m_ActiveScene->EndPlay(); // TODO: change this in the future
		}

		if (m_Framebuffer != nullptr)
		{
			delete m_Framebuffer;
			m_Framebuffer = nullptr;
		}
	}

	void SceneEditor::RenderGUI()
	{
		float deltaTime = Time::GetScaledDeltaTime();

		// render
		m_Framebuffer->Bind();
		if (m_ActiveScene != nullptr)
		{
			if (ShouldUseEditorCamera())
			{
				const float aspectRatio = (float)m_Framebuffer->GetSpecification().Width / (float)m_Framebuffer->GetSpecification().Height;
				m_CameraController.SetAspectRatio(aspectRatio);
				SceneRenderer::Render(*m_ActiveScene, &m_CameraController.GetCamera());
			}
			else
			{
				SceneRenderer::Render(*m_ActiveScene);
			}
		}
		else
		{
			Renderer2D::Begin(nullptr);
			Renderer2D::End();
		}
		m_Framebuffer->Unbind();

		// update
		if (m_ActiveScene != nullptr)
		{
			if (ShouldUseEditorCamera())
			{
				m_CameraController.Update(deltaTime);
			}

			if (m_SceneState == PlaybackState::PLAYING)
			{
				m_ActiveScene->Update(deltaTime);
			}
			else
			{
				m_ActiveScene->Update(0.0f);
			}
		}

		m_DetailsPanel->SetContext(m_SceneOutlinerPanel->GetSelectedActor());

		Editor::RenderGUI();
	}

	void SceneEditor::PostRenderGUI()
	{
		Editor::PostRenderGUI();
		m_EditorLayer->SetBlockEventsEnabled(!m_ViewportPanel->IsHovered());
	}

	void SceneEditor::HandleEvent(Event& event)
	{
		Editor::HandleEvent(event);
		if (event.IsHandled()) return;

		if (m_ViewportPanel->IsHovered())
		{
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e)
				{
					if (e.GetButton() == SPX_BUTTON_RIGHT)
					{
						const Vector2u& size = m_ViewportPanel->GetSize();
						const Vector2u& position = m_ViewportPanel->GetPosition();
						Input::SetMouseGrabRect(position.X + 1, position.Y + 1, size.X - 2, size.Y - 2);
						Input::SetCursorVisible(false);
						//Input::SetMouseGrab(true);
					}
					return false;
				});
			dispatcher.Dispatch<MouseButtonReleasedEvent>([](MouseButtonReleasedEvent& e)
				{
					if (e.GetButton() == SPX_BUTTON_RIGHT)
					{
						Input::SetCursorVisible(true);
						Input::SetMouseGrabRect(0, 0, 0, 0);
						//Input::SetMouseGrab(false);
					}
					return false;
				});
		}

		if (m_ActiveScene != nullptr && ShouldUseEditorCamera())
		{
			m_CameraController.HandleEvent(event);
		}
	}

	void SceneEditor::RenderMenuBar()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Scene...", nullptr, nullptr))
			{
				auto path = Sphynx::FileDialog::Open();
				if (!path.empty())
				{
					OpenScene(path);
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save", nullptr, nullptr, m_ActiveScene != nullptr))
			{
				SaveScene();
			}

			if (ImGui::MenuItem("Save As...", nullptr, nullptr, m_ActiveScene != nullptr))
			{
				auto path = Sphynx::FileDialog::Save();
				if (!path.empty())
				{
					SaveAsScene(path);
				}
			}

			ImGui::EndMenu();
		}
	}

	void SceneEditor::ManageProjectOpened(const ProjectInfo& projectInfo)
	{
		OpenScene(projectInfo.InitialScene);
	}

	void SceneEditor::OpenPrefabEditor(Prefab* prefab)
	{
		PrefabEditor* prefabEditor = new PrefabEditor(prefab);
		m_EditorLayer->AddEditor(prefabEditor);
		m_EditorLayer->SetActiveEditor(prefabEditor);
	}

	void SceneEditor::OpenPropertyEditor(const Reflection::Class& reflectionClass, void* object)
	{
		PropertyEditor* propertyEditor = new PropertyEditor(reflectionClass, object);
		m_EditorLayer->AddEditor(propertyEditor);
		m_EditorLayer->SetActiveEditor(propertyEditor);
	}

	void SceneEditor::OpenScene(const std::filesystem::path& path)
	{
		// TODO: check whether or not the loaded scene is registered in the asset manager

		m_LastOpenedScenePath = path;
		m_SceneToEdit = {};

		YAMLReader reader{ m_LastOpenedScenePath };
		EditorSceneDeserializer deserializer{ m_SceneToEdit, reader };
		deserializer.Deserialize();

		m_ActiveScene = &m_SceneToEdit;
		m_SceneOutlinerPanel->SetContext(m_ActiveScene);
	}

	void SceneEditor::SaveScene()
	{
		YAMLWriter writer{ m_LastOpenedScenePath };
		EditorSceneSerializer serializer{ m_SceneToEdit, writer };
		serializer.Serialize();
	}

	void SceneEditor::SaveAsScene(const std::filesystem::path& path)
	{
		// Invalid means it is not a "replace" save as request
		AssetMetadata metadata = AssetManager::GetAssetMetadataFromPath(path);
		if (metadata == AssetMetadata::Invalid)
		{
			metadata.Handle = AssetHandle::Generate();
			metadata.Path = path;
			metadata.Path.replace_extension(ASSET_EXTENSION);
			metadata.Type = TypeToAssetType<Scene>::Value();
			AssetManager::AddToRegistry(metadata);
		}

		std::string sceneName = path.filename().string();
		m_SceneToEdit.SetName(sceneName);

		// Important to have this in a scope to close any opened files
		{
			YAMLWriter writer{ metadata.Path };
			EditorSceneSerializer serializer{ m_SceneToEdit, writer };
			serializer.Serialize();
		}

		OpenScene(path);
	}

	void SceneEditor::PlayScene()
	{
		m_SceneToPlay = m_SceneToEdit;
		m_ActiveScene = &m_SceneToPlay;
		m_ActiveScene->BeginPlay();
		m_SceneOutlinerPanel->SetContext(m_ActiveScene);

		ResumeScene();

		DeactivateEject();
	}

	void SceneEditor::PauseScene()
	{
		m_SceneState = PlaybackState::PAUSED;
		m_SceneToolbar->SetPlaybackState(m_SceneState);
	}

	void SceneEditor::ResumeScene()
	{
		m_SceneState = PlaybackState::PLAYING;
		m_SceneToolbar->SetPlaybackState(m_SceneState);
	}

	void SceneEditor::StopScene()
	{
		m_SceneState = PlaybackState::STOPPED;
		m_SceneToolbar->SetPlaybackState(m_SceneState);

		m_ActiveScene->EndPlay();
		m_ActiveScene = &m_SceneToEdit;
		m_SceneOutlinerPanel->SetContext(m_ActiveScene);

		DeactivateEject();
	}

	void SceneEditor::ActivateEject()
	{
		m_Ejected = true;
		m_SceneToolbar->GetEjectButton()->UV0 = Vector2f(1.0f, 1.0f);
		m_SceneToolbar->GetEjectButton()->UV1 = Vector2f(0.0f, 0.0f);
	}

	void SceneEditor::DeactivateEject()
	{
		m_Ejected = false;
		m_SceneToolbar->GetEjectButton()->UV0 = Vector2f(0.0f, 0.0f);
		m_SceneToolbar->GetEjectButton()->UV1 = Vector2f(1.0f, 1.0f);
	}

	bool SceneEditor::ShouldUseEditorCamera()
	{
		return m_SceneState == STOPPED || m_Ejected;
	}
}