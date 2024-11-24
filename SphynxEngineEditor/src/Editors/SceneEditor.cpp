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

// TODO: remove
#include <Sphynx.h>
#include <Renderer/OrthographicCameraController.h>
#include "Asset/AssetManager.h"
#include <Renderer/Sprite.h>

#include <imgui.h>


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
		m_SceneToEdit(Scene()),
		m_SceneToPlay(Scene()),
		m_ActiveScene(nullptr),
		m_SceneState(PlaybackState::STOPPED),
		m_SceneNameBuffer()
	{
		AddWidget(m_SceneToolbar);
		AddWidget(m_SceneOutlinerPanel);
		AddWidget(m_ContentBrowserPanel);
		AddWidget(m_ViewportPanel);
		AddWidget(m_DetailsPanel);

		m_Framebuffer = Framebuffer::Create({ 1920, 1080, { FramebufferTextureFormat::RGBA8 } });
		m_ViewportPanel->SetFramebuffer(m_Framebuffer);
		m_ViewportPanel->SetIndex(0);

		// TODO: remove (testing)
		{
#if 0
			Actor quad = s_Scene.CreateActor();
			quad.AddComponent<NameComponent>("Hollow Box");
			quad.AddComponent<TransformComponent>(Transform{ { 0, 0, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
			quad.AddComponent<BoxRendererComponent>();

			Actor line = s_Scene.CreateActor();
			line.AddComponent<NameComponent>("White Line");
			line.AddComponent<TransformComponent>(Transform{ { 0.5f, -0.5f, 0.0f }, { 1.0f, 2.0f, 1.0f }, { 0.0f, 0.0f, 45.0f } });
			line.AddComponent<LineRendererComponent>();

			Actor staticRigidBody = s_Scene.CreateActor();
			staticRigidBody.AddComponent<NameComponent>("Ground");
			staticRigidBody.AddComponent<TransformComponent>(Transform{ { 0.0f, -2.0f, 0.0f }, { 5.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
			staticRigidBody.AddComponent<Rigidbody2DComponent>().SetRigidbodyType(RigidbodyType::STATIC);
			staticRigidBody.AddComponent<BoxCollider2DComponent>(Vector2f{ 1.0f, 1.0f }, Vector2f{ 0.0f, 1.0f });

			Actor dynamicRigidbody = s_Scene.CreateActor();
			dynamicRigidbody.AddComponent<NameComponent>("Box");
			dynamicRigidbody.AddComponent<TransformComponent>(Transform{ { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 45.0f } });
			dynamicRigidbody.AddComponent<Rigidbody2DComponent>().SetRigidbodyType(RigidbodyType::DYNAMIC);
			dynamicRigidbody.AddComponent<BoxCollider2DComponent>(Vector2f{ 1.0f, 1.0f }, Vector2f{ 0.0f, 0.0f });

			Actor dynamicSphereRigidbody = s_Scene.CreateActor();
			dynamicSphereRigidbody.AddComponent<NameComponent>("Sphere");
			dynamicSphereRigidbody.AddComponent<TransformComponent>(Transform{ { 0.5f, 3.0f, 0.0f }, { 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 35.0f } });
			dynamicSphereRigidbody.AddComponent<Rigidbody2DComponent>().SetRigidbodyType(RigidbodyType::DYNAMIC);
			dynamicSphereRigidbody.AddComponent<CircleCollider2DComponent>(1.0f, Vector2f{ 0.0f, 0.0f });

			Actor capsuleRigidbody = s_Scene.CreateActor();
			capsuleRigidbody.AddComponent<NameComponent>("Capsule");
			capsuleRigidbody.AddComponent<TransformComponent>(Transform{ { -1.5f, 1.0f, 0.0f }, { 2.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, -10.0f } });
			capsuleRigidbody.AddComponent<Rigidbody2DComponent>().SetRigidbodyType(RigidbodyType::DYNAMIC);
			capsuleRigidbody.AddComponent<CapsuleCollider2DComponent>(Vector2f{ 1.0f, 2.0f }, Vector2f{ 1.5f, 0.0f });
#else
			YAMLReader reader{ "Assets\\Scenes\\scene1.txt" };
			SceneDeserializer sceneDeserializer{ m_SceneToEdit, reader };
			sceneDeserializer.Deserialize();
#endif
		}

		m_ActiveScene = &m_SceneToEdit;
		m_SceneToolbar->GetStopButton()->IsEnabled = false;
		m_SceneOutlinerPanel->SetContext(m_ActiveScene);

		// save button
		m_SceneToolbar->GetSaveButton()->OnClick.Bind([&]() {
			YAMLWriter writer{ "Assets\\Scenes\\" + m_ActiveScene->GetName() + ".txt" };
			SceneSerializer sceneSerializer{ *m_ActiveScene, writer };
			sceneSerializer.Serialize();
		});

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
		m_SceneToolbar->GetStopButton()->OnClick.Bind([&]() {
			StopScene();
		});
	}

	SceneEditor::~SceneEditor()
	{
		m_ActiveScene->EndPlay(); // TODO: change this in the future
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
			Renderer2D::Begin(&s_CameraController.GetCamera());
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