#include "spxpch.h"
#include "EditorLayer.h"

#include "Events/Event.h"
#include "Renderer/Renderer.h"
#include "Platform/SDL/SDLEditorLayer.h"
#include "Platform/OpenGL/OpenGLEditorLayer.h"
#include "Dialogs/FileDialog.h"
#include "Base/Widget.h"
#include "Editors/SceneEditor.h"
#include "Editors/ProjectEditor.h"
//#include "Editors/PrefabEditor.h"

#include <imgui.h>


namespace Sphynx
{
	EditorLayer::EditorLayer() :
		m_BlockEventsEnabled(true),
		m_Editors({ new SceneEditor(this), new ProjectEditor() }),
		m_ActiveEditor(m_Editors[0]) // TODO: this is not being used right now
	{
	}

	EditorLayer::~EditorLayer()
	{
		for (Editor* editor : m_Editors)
		{
			delete editor;
		}
		m_Editors.clear();

		m_ActiveEditor = nullptr;
	}

	void EditorLayer::Attach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Viewports
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;   // Do not control cursor visibility

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:    SPX_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!"); break;
		case RendererAPI::API::SDL:		SDLEditorLayer::Attach(); break;
		case RendererAPI::API::OPENGL:	OpenGLEditorLayer::Attach(); break;
		default:						SPX_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}
	}

	void EditorLayer::Detach()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:    SPX_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!"); break;
		case RendererAPI::API::SDL:		SDLEditorLayer::Detach(); break;
		case RendererAPI::API::OPENGL:	OpenGLEditorLayer::Detach(); break;
		default:						SPX_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		ImGui::DestroyContext();
	}

	void EditorLayer::Update(float deltaTime)
	{
		for (Widget* widget : m_Editors)
		{
			widget->PreRenderUpdate(deltaTime);
		}

		Begin();
		RenderGUI();
		End();

		for (Widget* widget : m_Editors)
		{
			widget->PostRenderUpdate(deltaTime);
		}
	}

	void EditorLayer::HandleEvent(Event& event)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:    SPX_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!"); break;
		case RendererAPI::API::SDL:		SDLEditorLayer::HandleEvent(event); break;
		case RendererAPI::API::OPENGL:	OpenGLEditorLayer::HandleEvent(event); break;
		default:						SPX_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		event.SetHandled(false);
		if (m_BlockEventsEnabled)
		{
			ImGuiIO& io = ImGui::GetIO();
			const bool mouseCaptured = event.IsInCategory(Sphynx::MouseEvent) && io.WantCaptureMouse;
			event.SetHandled(event.IsHandled() || mouseCaptured);

			const bool keyboardCaptured = event.IsInCategory(Sphynx::KeyboardEvent) && (io.WantCaptureKeyboard || io.WantTextInput);
			event.SetHandled(event.IsHandled() || keyboardCaptured);
		}

		// If event is already handled by ImGui, we should not broadcast it to the rest of the app
		if (event.IsHandled()) return;

		for (Widget* widget : m_Editors)
		{
			widget->HandleEvent(event);
			if (event.IsHandled())
				break;
		}
	}

	void EditorLayer::Begin()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:    SPX_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!");  break;
		case RendererAPI::API::SDL:		SDLEditorLayer::Begin();  break;
		case RendererAPI::API::OPENGL:	OpenGLEditorLayer::Begin();  break;
		default:						SPX_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}

		ImGui::NewFrame();
	}

	void EditorLayer::End()
	{
		ImGui::Render();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:    SPX_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!"); break;
		case RendererAPI::API::SDL:		SDLEditorLayer::End();  break;
		case RendererAPI::API::OPENGL:	OpenGLEditorLayer::End();  break;
		default:						SPX_CORE_ASSERT(false, "Unknown RendererAPI!"); break;
		}
	}

	void EditorLayer::RenderGUI()
	{
		static bool showDemoWindow = false;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Sphynx Editor", nullptr, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EditorDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		const size_t num = m_Editors.size();
		for (int i = 0; i < num; ++i)
		{
			Widget* widget = m_Editors[i];
			widget->PreRenderGUI();
			widget->RenderGUI();
			widget->PostRenderGUI();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit"))
				{
					Application::GetInstance()->Quit();
				}
				ImGui::EndMenu();
			}

		if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("Show Demo Window", nullptr, &showDemoWindow, true);
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		ImGui::End();
	}

	bool EditorLayer::IsBlockEventsEnabled() const
	{
		return m_BlockEventsEnabled;
	}

	void EditorLayer::SetBlockEventsEnabled(bool enabled)
	{
		m_BlockEventsEnabled = enabled;
	}

	void EditorLayer::AddEditor(Editor* editor)
	{
		m_Editors.push_back(editor);
	}

	void EditorLayer::RemoveEditor(Editor* editor)
	{
		auto it = std::find(m_Editors.begin(), m_Editors.end(), editor);
		if (it == m_Editors.end()) return;

		m_Editors.erase(it);
	}

	void EditorLayer::SetActiveEditor(Editor* editor)
	{
		m_ActiveEditor = editor;
	}
}
