#include "spxpch.h"
#include "ViewportPanel.h"

#include <imgui.h>
#include <SDL3/SDL.h>
#include <EditorLayer.h>
#include "imgui_internal.h"


namespace Sphynx
{
	ViewportPanel::ViewportPanel(Widget* parent) :
		ViewportPanel(nullptr, 0u, parent)
	{
	}

	ViewportPanel::ViewportPanel(Framebuffer* framebuffer, uint32_t index, Widget* parent) :
		Panel("Viewport", parent),
		m_Framebuffer(framebuffer),
		m_Index(index),
		m_AvailableSize()
	{
	}

	ViewportPanel::~ViewportPanel()
	{
		m_Framebuffer = nullptr;
		m_Index = 0u;
	}

	void ViewportPanel::SetFramebuffer(Framebuffer* framebuffer)
	{
		m_Framebuffer = framebuffer;
	}

	void ViewportPanel::SetIndex(uint32_t index)
	{
		m_Index = index;
	}

	void ViewportPanel::PreRenderGUI()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });
		Panel::PreRenderGUI();
	}

	void ViewportPanel::RenderGUI()
	{
		if (!m_CanRender) return;

		if (m_Framebuffer != nullptr)
		{
			m_AvailableSize = { (int32_t)ImGui::GetContentRegionAvail().x, (int32_t)ImGui::GetContentRegionAvail().y };
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f, 0.0f });
			ImGui::ImageButtonEx(
				GetID(),
				(ImTextureID)m_Framebuffer->GetColorAttachment(m_Index),
				ImGui::GetContentRegionAvail(),
				ImVec2{ 0, 0 },			// uv0
				ImVec2{ 1, 1 },			// uv1
				ImVec4{ 0, 0, 0, 0 },	// bg color
				ImVec4{ 1, 1, 1, 1 });	// tint color
			ImGui::PopStyleVar();
		}
	}

	void ViewportPanel::PostRenderGUI()
	{
		Panel::PostRenderGUI();
		ImGui::PopStyleVar();
	}

	void ViewportPanel::PostRenderUpdate(float deltaTime)
	{
		CheckFramebufferSizeValidity();
	}

	void ViewportPanel::CheckFramebufferSizeValidity()
	{
		if (m_Framebuffer == nullptr) return;

		Vector2i framebufferSize{ (int32_t)m_Framebuffer->GetSpecification().Width, (int32_t)m_Framebuffer->GetSpecification().Height };

		if (m_AvailableSize != Vector2i::Zero && framebufferSize != m_AvailableSize)
		{
			m_Framebuffer->Resize(m_AvailableSize.X, m_AvailableSize.Y);
		}
	}
}
