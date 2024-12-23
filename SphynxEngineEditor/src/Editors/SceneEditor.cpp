#include "spxpch.h"
#include "SceneEditor.h"

#include "Base/Resources.h"
#include "Panels/SceneToolbar.h"
#include "Panels/SceneOutlinerPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/DetailsPanel.h"

#include "Widgets/ButtonWidget.h"

#include "Core/Time.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Framebuffer.h"

#include "Asset/Scene/SceneAsset.h"

// TODO: remove
#include <Sphynx.h>
#include <Renderer/OrthographicCameraController.h>
#include "Asset/AssetManager.h"
#include <Renderer/Sprite.h>

#include <imgui.h>
#include "Dialogs/FileDialog.h"


namespace Sphynx
{
	// TODO: remove, this is just for testing purposes
	namespace
	{
		static OrthographicCameraController s_CameraController(1.0f, true);
	}

	SceneEditor::SceneEditor() :
		Editor("SceneEditor"),
		m_SceneToolbar(new SceneToolbar()),
		m_SceneOutlinerPanel(new SceneOutlinerPanel()),
		m_ContentBrowserPanel(new ContentBrowserPanel()),
		m_ViewportPanel(new ViewportPanel()),
		m_DetailsPanel(new DetailsPanel()),
		m_Framebuffer(nullptr),
		m_LastOpenedScenePath(),
		m_SceneToEdit(),
		m_SceneToPlay(),
		m_ActiveScene(nullptr),
		m_SceneState(PlaybackState::STOPPED),
		m_SceneNameBuffer()
	{
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
	}

	SceneEditor::~SceneEditor()
	{
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

		// TODO: change by a EditorCameraController
		s_CameraController.Update(deltaTime);
		uint32_t width = m_Framebuffer->GetSpecification().Width;
		uint32_t height = m_Framebuffer->GetSpecification().Height;
		s_CameraController.Resize((float)width, (float)height);

		m_Framebuffer->Bind();
		{
			// begin scene render
			Renderer2D::Begin(s_CameraController.GetCamera().GetCamera());
			if (m_ActiveScene != nullptr)
			{
				if (m_SceneState == PlaybackState::PLAYING)
				{
					m_ActiveScene->Update(deltaTime);
				}
				else
				{
					m_ActiveScene->Update(0.0f);
				}
			}
			Renderer2D::End();
		}
		m_Framebuffer->Unbind();

		m_DetailsPanel->SetContext(m_SceneOutlinerPanel->GetSelectedActor());

		Editor::RenderGUI();
	}

	void SceneEditor::RenderMenuBar()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open...", nullptr, nullptr))
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

	void SceneEditor::OpenScene(const std::filesystem::path& path)
	{
		// TODO: check whether or not the loaded scene is 
		// registered in the asset manager

		m_LastOpenedScenePath = path;
		m_SceneToEdit = {};

		YAMLReader reader{ m_LastOpenedScenePath };
		SceneDeserializer deserializer{ m_SceneToEdit, reader };
		deserializer.Deserialize();

		m_ActiveScene = &m_SceneToEdit;
		m_SceneOutlinerPanel->SetContext(m_ActiveScene);
	}

	void SceneEditor::SaveScene()
	{
		YAMLWriter writer{ m_LastOpenedScenePath };
		SceneSerializer serializer{ m_SceneToEdit, writer };
		serializer.Serialize();
	}

	void SceneEditor::SaveAsScene(const std::filesystem::path& path)
	{
		// Invalid means it is not a "replace" save as request
		AssetMetadata metadata = AssetManager::GetMetadataFromPath(path);
		if (metadata == AssetMetadata::Invalid)
		{
			metadata.Handle = AssetHandle::Generate();
			metadata.Path = path;
			metadata.Path.replace_extension(ASSET_EXTENSION);
			metadata.Type = TypeToAssetType<Scene>::Value;
			AssetManager::AddToRegistry(metadata);
		}

		std::string sceneName = path.filename().string();
		m_SceneToEdit.SetName(sceneName);

		// Important to have this in a scope to close any opened files
		{
			YAMLWriter writer{ metadata.Path };
			SceneSerializer serializer{ m_SceneToEdit, writer };
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
	}
}