#include "spxpch.h"
#include "ViewportPanel.h"

#include <imgui.h>
#include <SDL3/SDL.h>
#include <EditorLayer.h>


namespace Sphynx
{
	ViewportPanel::ViewportPanel() :
		ViewportPanel(nullptr, 0u)
	{
	}

	ViewportPanel::ViewportPanel(Framebuffer* framebuffer, uint32_t index) :
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

	void ViewportPanel::RenderGUI()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });
		if (ImGui::Begin("Viewport"))
		{
			if (m_Framebuffer != nullptr)
			{
				m_AvailableSize = { (int32_t)ImGui::GetContentRegionAvail().x, (int32_t)ImGui::GetContentRegionAvail().y };
				ImGui::Image((ImTextureID)m_Framebuffer->GetColorAttachment(m_Index), ImGui::GetContentRegionAvail());
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ViewportPanel::PostRenderGUI()
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
