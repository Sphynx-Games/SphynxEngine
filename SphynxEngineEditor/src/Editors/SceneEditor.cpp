#include "spxpch.h"
#include "SceneEditor.h"

#include "Panels/SceneOptionsPanel.h"
#include "Panels/SceneOutlinerPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/DetailsPanel.h"

//#include "Widgets/ButtonWidget.h"

#include "Core/Time.h"
#include "Scene/Scene.h"
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
		m_SceneOptionsPanel(new SceneOptionsPanel()),
		m_SceneOutlinerPanel(new SceneOutlinerPanel()),
		m_ContentBrowserPanel(new ContentBrowserPanel()),
		m_ViewportPanel(new ViewportPanel()),
		m_DetailsPanel(new DetailsPanel()),
		m_Framebuffer(nullptr),
		m_ActiveScene(nullptr),
		m_SceneNameBuffer()
	{
		AddWidget(m_SceneOptionsPanel);
		AddWidget(m_SceneOutlinerPanel);
		AddWidget(m_ContentBrowserPanel);
		AddWidget(m_ViewportPanel);
		AddWidget(m_DetailsPanel);

		m_Framebuffer = Framebuffer::Create({ 1920, 1080, { FramebufferTextureFormat::RGBA8 } });
		m_ViewportPanel->SetFramebuffer(m_Framebuffer);
		m_ViewportPanel->SetIndex(0);

		// TODO: remove (testing)
		{
			static Scene s_Scene;
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
			SceneDeserializer sceneDeserializer{ s_Scene, reader };
			sceneDeserializer.Deserialize();
#endif
			m_ActiveScene = &s_Scene;

			// set the execute function of the SaveButtonWidget in SceneOptionsPanel
			/*m_SceneOptionsPanel->GetSaveButton()->OnClick.Bind([&]() {

				FileWriter writer = FileWriter("Assets\\Scenes\\" + m_ActiveScene->GetName() + ".txt");
				SceneSerializer sceneSerializer = SceneSerializer(*m_ActiveScene, writer);
				sceneSerializer.Serialize();
				
			});*/

			m_ActiveScene->BeginPlay(); // TODO: change this in the future
		}

		m_SceneOptionsPanel->SetContext(m_ActiveScene);
		m_SceneOutlinerPanel->SetContext(m_ActiveScene);
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
				m_ActiveScene->Update(deltaTime);
			}
			Renderer2D::End();
		}
		m_Framebuffer->Unbind();

		m_DetailsPanel->SetContext(m_SceneOutlinerPanel->GetSelectedActor());

		Editor::RenderGUI();
	}
}