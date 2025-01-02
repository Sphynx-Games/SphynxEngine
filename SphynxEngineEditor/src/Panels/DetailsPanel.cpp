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

			// Draw Camera Component
			if (m_Context.HasComponent<CameraComponent>())
			{
				if (ImGui::CollapsingHeader("Camera Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<CameraComponent>();
					const char* mode = component.GetProjectionMode() == CameraProjectionMode::ORTHOGRAPHIC ? "ORTHOGRAPHIC" : "PERSPECTIVE";
					if (ImGui::BeginCombo(LABEL("Projection Mode").c_str(), mode))
					{
						if (ImGui::Selectable("ORTHOGRAPHIC", false)) component.SetProjectionMode(CameraProjectionMode::ORTHOGRAPHIC);
						if (ImGui::Selectable("PERSPECTIVE", false)) component.SetProjectionMode(CameraProjectionMode::PERSPECTIVE);
						ImGui::EndCombo();
					}

					// Near
					{
						float value = component.GetNear();
						if (ImGui::DragFloat(LABEL("Near").c_str(), &value, 1.0f, 1e-6f, component.GetFar()))
						{
							component.SetNear(value);
						}
					}

					// Far
					{
						float value = component.GetFar();
						if (ImGui::DragFloat(LABEL("Far").c_str(), &value, 1.0f, component.GetNear(), FLT_MAX))
						{
							component.SetFar(value);
						}
					}

					// HeightUnits
					if (component.GetProjectionMode() == CameraProjectionMode::ORTHOGRAPHIC)
					{
						float value = component.GetHeightUnits();
						if (ImGui::DragFloat(LABEL("Height Units").c_str(), &value, 1.0f, 0.0f, FLT_MAX))
						{
							component.SetHeightUnits(value);
						}
					}

					// FieldOfView
					if (component.GetProjectionMode() == CameraProjectionMode::PERSPECTIVE)
					{
						float value = component.GetFieldOfView();
						if (ImGui::DragFloat(LABEL("Field Of View").c_str(), &value, 1.0f, 0.0f, 360.0f))
						{
							component.SetFieldOfView(value);
						}
					}

					// Is Main Camera
					ImGui::Checkbox(LABEL("Is Main Camera").c_str(), &component.IsMainCamera);

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
						// TODO: list all possible sprites
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
					component.Tint.R = (uint8_t)(color[0] * 255.0f);
					component.Tint.G = (uint8_t)(color[1] * 255.0f);
					component.Tint.B = (uint8_t)(color[2] * 255.0f);
					component.Tint.A = (uint8_t)(color[3] * 255.0f);

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
					component.Color.R = (uint8_t)(color[0] * 255.0f);
					component.Color.G = (uint8_t)(color[1] * 255.0f);
					component.Color.B = (uint8_t)(color[2] * 255.0f);
					component.Color.A = (uint8_t)(color[3] * 255.0f);

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
					component.Color.R = (uint8_t)(color[0] * 255.0f);
					component.Color.G = (uint8_t)(color[1] * 255.0f);
					component.Color.B = (uint8_t)(color[2] * 255.0f);
					component.Color.A = (uint8_t)(color[3] * 255.0f);

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

			// Draw Rigidbody2DComponent
			if (m_Context.HasComponent<Rigidbody2DComponent>())
			{
				if (ImGui::CollapsingHeader("Rigidbody2D Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<Rigidbody2DComponent>();

					bool enabled = component.IsEnabled();
					if (ImGui::Checkbox(LABEL("Enabled").c_str(), &enabled))
					{
						component.SetEnabled(enabled);
					}

					RigidbodyType type = component.GetRigidbodyType();
					const char* mode = "STATIC";
					switch (type)
					{
					case RigidbodyType::STATIC:    mode = "STATIC"; break;
					case RigidbodyType::DYNAMIC:  mode = "DYNAMIC"; break;
					case RigidbodyType::KINEMATIC: mode = "KINEMATIC"; break;
					}

					if (ImGui::BeginCombo(LABEL("Type").c_str(), mode))
					{
						if (ImGui::Selectable("STATIC", false)) component.SetRigidbodyType(RigidbodyType::STATIC);
						if (ImGui::Selectable("DYNAMIC", false)) component.SetRigidbodyType(RigidbodyType::DYNAMIC);
						if (ImGui::Selectable("KINEMATIC", false)) component.SetRigidbodyType(RigidbodyType::KINEMATIC);
						ImGui::EndCombo();
					}

					Vector2f linearVelocity = component.GetLinearVelocity();
					if (ImGui::DragFloat2(LABEL("Linear Velocity").c_str(), (float*)&linearVelocity, 0.1f))
					{
						component.SetLinearVelocity(linearVelocity);
					}

					float angularVelocity = component.GetAngularVelocity();
					if (ImGui::DragFloat(LABEL("Angular Velocity").c_str(), &angularVelocity, 0.1f))
					{
						component.SetAngularVelocity(angularVelocity);
					}

					float linearDamping = component.GetLinearDamping();
					if (ImGui::DragFloat(LABEL("Linear Damping").c_str(), &linearDamping, 0.1f))
					{
						component.SetLinearDamping(linearDamping);
					}

					float angularDamping = component.GetAngularDamping();
					if (ImGui::DragFloat(LABEL("Angular Damping").c_str(), &angularDamping, 0.1f))
					{
						component.SetAngularDamping(angularDamping);
					}

					float gravityScale = component.GetGravityScale();
					if (ImGui::DragFloat(LABEL("Gravity Scale").c_str(), &gravityScale, 0.1f))
					{
						component.SetGravityScale(gravityScale);
					}

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

			// Draw BoxCollider2DComponent
			if (m_Context.HasComponent<BoxCollider2DComponent>())
			{
				if (ImGui::CollapsingHeader("BoxCollider2D Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<BoxCollider2DComponent>();

					Vector2f size = component.GetSize();
					if (ImGui::DragFloat2(LABEL("Size").c_str(), (float*)&size, 0.1f))
					{
						component.SetSize(size);
					}

					Vector2f offset = component.GetOffset();
					if (ImGui::DragFloat2(LABEL("Offset").c_str(), (float*)&offset, 0.1f))
					{
						component.SetOffset(offset);
					}

					bool trigger = component.IsTrigger();
					if (ImGui::Checkbox(LABEL("Trigger").c_str(), &trigger))
					{
						component.SetIsTrigger(trigger);
					}

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

			// Draw CircleCollider2DComponent
			if (m_Context.HasComponent<CircleCollider2DComponent>())
			{
				if (ImGui::CollapsingHeader("CircleCollider2D Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<CircleCollider2DComponent>();

					float radius = component.GetRadius();
					if (ImGui::DragFloat(LABEL("Radius").c_str(), &radius, 0.1f))
					{
						component.SetRadius(radius);
					}

					Vector2f offset = component.GetOffset();
					if (ImGui::DragFloat2(LABEL("Offset").c_str(), (float*)&offset, 0.1f))
					{
						component.SetOffset(offset);
					}

					bool trigger = component.IsTrigger();
					if (ImGui::Checkbox(LABEL("Trigger").c_str(), &trigger))
					{
						component.SetIsTrigger(trigger);
					}

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

			// Draw CapsuleCollider2DComponent
			if (m_Context.HasComponent<CapsuleCollider2DComponent>())
			{
				if (ImGui::CollapsingHeader("CapsuleCollider2D Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(ImGui::GetStyle().IndentSpacing);

					auto& component = m_Context.GetComponent<CapsuleCollider2DComponent>();

					Vector2f size = component.GetSize();
					if (ImGui::DragFloat2(LABEL("Size").c_str(), (float*)&size, 0.1f))
					{
						component.SetSize(size);
					}

					Vector2f offset = component.GetOffset();
					if (ImGui::DragFloat2(LABEL("Offset").c_str(), (float*)&offset, 0.1f))
					{
						component.SetOffset(offset);
					}

					bool trigger = component.IsTrigger();
					if (ImGui::Checkbox(LABEL("Trigger").c_str(), &trigger))
					{
						component.SetIsTrigger(trigger);
					}

					ImGui::Unindent(ImGui::GetStyle().IndentSpacing);
				}
			}

		}
		ImGui::End();
	}
}