#pragma once

#include "Base/Panel.h"
#include "Math/Vector.h"


namespace Sphynx
{
	class Framebuffer;

	class ViewportPanel : public Panel
	{
	public:
		ViewportPanel(Widget* parent = nullptr);
		ViewportPanel(Framebuffer* framebuffer, uint32_t index = 0u, Widget* parent = nullptr);
		virtual ~ViewportPanel();

		void SetFramebuffer(Framebuffer* framebuffer);
		void SetIndex(uint32_t index);

	protected:
		virtual void PreRenderGUI() override;
		virtual void RenderGUI() override;
		virtual void PostRenderGUI() override;

		virtual void PostRenderUpdate(float deltaTime) override;

	private:
		void CheckFramebufferSizeValidity();

	private:
		Framebuffer* m_Framebuffer;
		uint32_t m_Index;

		Vector2i m_AvailableSize;

	};
}