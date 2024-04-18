#include "spxpch.h"
#include "SceneEditor.h"

#include "Panels/SceneOutlinerPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/DetailsPanel.h"

#include "Core/Time.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Framebuffer.h"

// TODO: remove
#include <Sphynx.h>
#include <Renderer/OrthographicCameraController.h>
#include "Asset/AssetManager.h"
#include <Renderer/Sprite.h>

namespace Sphynx
{
	// TODO: remove, this is just for testing purposes
	namespace
	{
		static OrthographicCameraController s_CameraController(1.0f, true);
		static Scene s_Scene;
	}

	SceneEditor::SceneEditor() :
		Editor("SceneEditor"),
		m_SceneOutlinerPanel(new SceneOutlinerPanel()),
		m_ContentBrowserPanel(new ContentBrowserPanel()),
		m_ViewportPanel(new ViewportPanel()),
		m_DetailsPanel(new DetailsPanel()),
		m_Framebuffer(nullptr),
		m_ActiveScene(nullptr)
	{
		AddPanel(m_SceneOutlinerPanel);
		AddPanel(m_ContentBrowserPanel);
		AddPanel(m_ViewportPanel);
		AddPanel(m_DetailsPanel);

		m_Framebuffer = Framebuffer::Create({ 1920, 1080, { FramebufferTextureFormat::RGBA8 } });
		m_ViewportPanel->SetFramebuffer(m_Framebuffer);
		m_ViewportPanel->SetIndex(0);

		// TODO: remove (testing)
		{
			AssetManager::Import("..\\Assets\\Textures\\cat.jpg");
			Texture* enemyTexture = *AssetManager::Import<Texture>("..\\Assets\\Textures\\enemy_scaled5x.png");
			Spritesheet* sheet = new Spritesheet(enemyTexture, 4, 3);

			Actor sprt = s_Scene.CreateActor();
			sprt.AddComponent<NameComponent>("Sprite Actor");
			sprt.AddComponent<TransformComponent>(Transform{ { 0, 0, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
			sprt.AddComponent<SpriteRendererComponent>(sheet->GetSprite(0), Color::Blue);

			Actor quad = s_Scene.CreateActor();
			quad.AddComponent<NameComponent>("Hollow Box");
			quad.AddComponent<TransformComponent>(Transform{ { 0, 0, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } });
			quad.AddComponent<BoxRendererComponent>();

			Actor line = s_Scene.CreateActor();
			line.AddComponent<NameComponent>("White Line");
			line.AddComponent<TransformComponent>(Transform{ { 0.5f, -0.5f, 0.0f }, { 1.0f, 2.0f, 1.0f }, { 0.0f, 0.0f, 45.0f } });
			line.AddComponent<LineRendererComponent>();

			m_ActiveScene = &s_Scene;
		}

		m_SceneOutlinerPanel->SetContext(m_ActiveScene);
	}

	SceneEditor::~SceneEditor()
	{
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