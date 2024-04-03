#include "EditorLayer.h"

#include "Base/Widget.h"

#include <imgui.h>
#include <Platform/SDL/SDLEditorLayer.h>
#include <Renderer/Renderer.h>
#include <Renderer/Renderer2D.h>


EditorLayer::~EditorLayer()
{
	for (Widget* widget : m_Widgets)
	{
		delete widget;
	}
	m_Widgets.clear();
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

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
}

void EditorLayer::Detach()
{
	ImGui::DestroyContext();
}

void EditorLayer::Update(float deltaTime)
{
	Begin();
	RenderGUI();
	End();
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
	ImGui::Begin("Editor DockSpace", nullptr, window_flags);
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

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Open", nullptr, nullptr);
			ImGui::Separator();
			ImGui::MenuItem("Save", nullptr, nullptr);

			if (ImGui::MenuItem("Quit", nullptr, false, false))
			{
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

	for (Widget* widget : m_Widgets)
	{
		widget->RenderGUI();
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

void EditorLayer::AddWidget(Widget* widget)
{
	m_Widgets.push_back(widget);
}

void EditorLayer::RemoveWidget(Widget* widget)
{
	auto it = std::find(m_Widgets.begin(), m_Widgets.end(), widget);
	if (it == m_Widgets.end()) return;

	m_Widgets.erase(it);
}

EditorLayer* EditorLayer::Create()
{
	switch (Sphynx::Renderer::GetAPI())
	{
	case Sphynx::RendererAPI::API::None:    SPX_LOG_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
	case Sphynx::RendererAPI::API::SDL:     return new SDLEditorLayer();
	}

	SPX_LOG_CORE_ERROR("Unknown RendererAPI!");
	return nullptr;
}
