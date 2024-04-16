#pragma once

#include "Base/Panel.h"


namespace Sphynx
{
	class Framebuffer;

	class ViewportPanel : public Panel
	{
	public:
		ViewportPanel();
		ViewportPanel(Framebuffer* framebuffer, uint32_t index = 0u);
		virtual ~ViewportPanel();

		void SetFramebuffer(Framebuffer* framebuffer);
		void SetIndex(uint32_t index);

	protected:
		virtual void RenderGUI() override;

	private:
		void CheckFramebufferSizeValidity();

	private:
		Framebuffer* m_Framebuffer;
		uint32_t m_Index;
	};
}