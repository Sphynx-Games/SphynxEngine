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
		m_Index(index)
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
		if (ImGui::Begin("Viewport"))
		{
			if (m_Framebuffer != nullptr)
			{
				CheckFramebufferSizeValidity();
				ImGui::Image((ImTextureID)m_Framebuffer->GetColorAttachment(m_Index), ImGui::GetContentRegionAvail());
			}
		}

		ImGui::End();
	}

	void ViewportPanel::CheckFramebufferSizeValidity()
	{
		if (m_Framebuffer == nullptr) return;

		ImVec2 size = ImGui::GetContentRegionAvail();
		Vector2u regionSize = { (uint32_t)size.x, (uint32_t)size.y };
		Vector2u framebufferSize = { m_Framebuffer->GetSpecification().Width, m_Framebuffer->GetSpecification().Height };

		if (framebufferSize != regionSize)
		{
			m_Framebuffer->Resize(regionSize.X, regionSize.Y);
		}
	}
}
