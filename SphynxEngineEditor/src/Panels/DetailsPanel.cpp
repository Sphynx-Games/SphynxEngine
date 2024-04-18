#include <spxpch.h>
#include "DetailsPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <Component/Components.h>
#include <Scene/Scene.h>


namespace Sphynx
{
#define LABEL(Label) LabelPrefix(Label)
	static std::string LabelPrefix(const char* label)
	{
		float width = ImGui::CalcItemWidth();

		float x = ImGui::GetCursorPosX();
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::SetNextItemWidth(-1);

		std::string labelStr = "##";
		labelStr += label;

		return labelStr;
	}

	DetailsPanel::DetailsPanel(const Actor& actor) :
		m_Context(actor)
	{
	}

	void DetailsPanel::SetContext(const Actor& context)
	{
		m_Context = context;
	}

	void DetailsPanel::RenderGUI()
	{
		if (ImGui::Begin("Details") && m_Context.IsValid())
		{
			// Draw Name Component
			if (m_Context.HasComponent<NameComponent>())
			{
				if (ImGui::CollapsingHeader("Name Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<NameComponent>();
					ImGui::InputText(LABEL("Name").c_str(), &component.Name);

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

			// Draw Transform Component
			if (m_Context.HasComponent<TransformComponent>())
			{
				if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<TransformComponent>();
					ImGui::DragFloat3(LABEL("Position").c_str(), (float*)&component.Transform.Position, 0.1f);
					ImGui::DragFloat3(LABEL("Rotation").c_str(), (float*)&component.Transform.Rotation, 0.1f);
					ImGui::DragFloat3(LABEL("Scale").c_str(), (float*)&component.Transform.Scale, 0.1f);

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

			// Draw Sprite Renderer Component
			if (m_Context.HasComponent<SpriteRendererComponent>())
			{
				if (ImGui::CollapsingHeader("Sprite Renderer Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<SpriteRendererComponent>();
					if (ImGui::BeginCombo(LABEL("Sprite").c_str(), "TODO: Some sprite"))
					{
						// TODO: list all posible sprites
						ImGui::EndCombo();
					}
					float color[4] =
					{
						component.Tint.R / 255.0f,
						component.Tint.G / 255.0f,
						component.Tint.B / 255.0f,
						component.Tint.A / 255.0f
					};
					ImGui::ColorEdit4(LABEL("Color").c_str(), color);
					component.Tint = Color(color[0] * 255, color[1] * 255, color[2] * 255, color[3] * 255);

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

			// Draw Box Renderer Component
			if (m_Context.HasComponent<BoxRendererComponent>())
			{
				if (ImGui::CollapsingHeader("Box Renderer Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<BoxRendererComponent>();
					const char* mode = component.DrawMode == FILLED ? "FILLED" : "WIREFRAME";
					if (ImGui::BeginCombo(LABEL("Draw Mode").c_str(), mode))
					{
						if (ImGui::Selectable("WIREFRAME", false)) component.DrawMode = WIREFRAME;
						if (ImGui::Selectable("FILLED", false)) component.DrawMode = FILLED;
						ImGui::EndCombo();
					}

					ImGui::DragFloat2(LABEL("Size").c_str(), (float*)&component.Size, 0.1f);
					ImGui::DragFloat2(LABEL("Pivot").c_str(), (float*)&component.Pivot, 0.1f);

					float color[4] =
					{
						component.Color.R / 255.0f,
						component.Color.G / 255.0f,
						component.Color.B / 255.0f,
						component.Color.A / 255.0f
					};
					ImGui::ColorEdit4(LABEL("Color").c_str(), color);
					component.Color = Color(color[0] * 255, color[1] * 255, color[2] * 255, color[3] * 255);

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

			// Draw Line Renderer Component
			if (m_Context.HasComponent<LineRendererComponent>())
			{
				if (ImGui::CollapsingHeader("Line Renderer Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<LineRendererComponent>();
					ImGui::DragFloat2(LABEL("Point A").c_str(), (float*)&component.Point1, 0.1f);
					ImGui::DragFloat2(LABEL("Point B").c_str(), (float*)&component.Point2, 0.1f);
					ImGui::DragFloat(LABEL("Line Width").c_str(), (float*)&component.LineWidth, 0.1f, 0.0f, std::numeric_limits<float>::max());

					float color[4] =
					{
						component.Color.R / 255.0f,
						component.Color.G / 255.0f,
						component.Color.B / 255.0f,
						component.Color.A / 255.0f
					};
					ImGui::ColorEdit4(LABEL("Color").c_str(), color);
					component.Color = Color(color[0] * 255, color[1] * 255, color[2] * 255, color[3] * 255);

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

		}
		ImGui::End();
	}
}