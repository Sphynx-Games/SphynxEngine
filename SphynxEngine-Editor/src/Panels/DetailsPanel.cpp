#include <spxpch.h>
#include "DetailsPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <typeindex>
#include <typeinfo>

#include <Reflection/Reflection.h>
#include <Component/Components.h>
#include <Scene/Scene.h>
#include <Asset/AssetManager.h>
#include <Asset/Sprite/SpriteAsset.h>
#include <Container/Array.h>
#include <Component/ComponentRegistry.h>
#include <ImGuiExtra.h>
#include <Base/Resources.h>


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

	template<typename T>
	void AddComponentType(Array<const Reflection::Class*>& list)
	{
		list.Add(&Reflection::GetClass<T>());
	}

	DetailsPanel::DetailsPanel(Widget* parent) :
		Panel("Details", parent),
		m_Context(),
		m_ComponetTypeToRemove(nullptr)
	{
	}

	DetailsPanel::DetailsPanel(const Actor& actor, Widget* parent) :
		Panel("Details", parent),
		m_Context(actor),
		m_ComponetTypeToRemove(nullptr)
	{
	}

	void DetailsPanel::SetContext(const Actor& context)
	{
		m_Context = context;
	}

	void DetailsPanel::RenderGUI()
	{
		if (ImGui::Begin(GetName()) && m_Context.IsValid())
		{
			Array<const Reflection::Class*> componentTypesToAdd;

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
				RenderCameraComponent();
			}
			else
			{
				AddComponentType<CameraComponent>(componentTypesToAdd);
			}

			// Draw Sprite Renderer Component
			if (m_Context.HasComponent<SpriteRendererComponent>())
			{
				RenderSpriteRendererComponent();
			}
			else
			{
				AddComponentType<SpriteRendererComponent>(componentTypesToAdd);
			}

			// Draw Box Renderer Component
			if (m_Context.HasComponent<BoxRendererComponent>())
			{
				RenderBoxRendererComponent();
			}
			else
			{
				AddComponentType<BoxRendererComponent>(componentTypesToAdd);
			}

			// Draw Line Renderer Component
			if (m_Context.HasComponent<LineRendererComponent>())
			{
				RenderLineRendererComponent();
			}
			else
			{
				AddComponentType<LineRendererComponent>(componentTypesToAdd);
			}

			// Draw Rigidbody2DComponent
			if (m_Context.HasComponent<Rigidbody2DComponent>())
			{
				RenderRigidbody2DComponent();
			}
			else
			{
				AddComponentType<Rigidbody2DComponent>(componentTypesToAdd);
			}

			// Draw BoxCollider2DComponent
			if (m_Context.HasComponent<BoxCollider2DComponent>())
			{
				RenderBoxCollider2DComponent();
			}
			else
			{
				AddComponentType<BoxCollider2DComponent>(componentTypesToAdd);
			}

			// Draw CircleCollider2DComponent
			if (m_Context.HasComponent<CircleCollider2DComponent>())
			{
				RenderCircleCollider2DComponent();
			}
			else
			{
				AddComponentType<CircleCollider2DComponent>(componentTypesToAdd);
			}

			// Draw CapsuleCollider2DComponent
			if (m_Context.HasComponent<CapsuleCollider2DComponent>())
			{
				RenderCapsuleCollider2DComponent();
			}
			else
			{
				AddComponentType<CapsuleCollider2DComponent>(componentTypesToAdd);
			}

			// PANEL CONTEXT MENU
			if (ImGui::BeginPopupContextWindow("DetailsPanel_ContextMenu", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
			{
				if (ImGui::BeginMenu("New component"))
				{
					for (const auto& t : componentTypesToAdd)
					{
						std::string compName = t->Name;
						size_t pos = compName.find("::") + 2;
						compName = compName.substr(pos);
						if (ImGui::MenuItem(compName.c_str()))
						{
							ComponentRegistry::InvokeAddComponent(*t, m_Context);
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}

			// delete selected component
			if (m_ComponetTypeToRemove != nullptr)
			{
				ComponentRegistry::InvokeRemoveComponent(*m_ComponetTypeToRemove, m_Context);
				m_ComponetTypeToRemove = nullptr;
			}
		}
		ImGui::End();
	}

	void DetailsPanel::RenderCameraComponent()
	{
		const char* id = "Camera Component";
		bool opened = ImGui::CollapsingHeader(id, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
		RenderComponent_ContextMenu(id, Reflection::GetClass<CameraComponent>());
		
		if (opened)
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
	
	void DetailsPanel::RenderSpriteRendererComponent()
	{
		const char* id = "Sprite Renderer Component";
		bool opened = ImGui::CollapsingHeader(id, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
		RenderComponent_ContextMenu(id, Reflection::GetClass<SpriteRendererComponent>());

		if (opened)
		{
			ImGui::Indent(ImGui::GetStyle().IndentSpacing);

			SpriteRendererComponent& component = m_Context.GetComponent<SpriteRendererComponent>();
			AssetMetadata selectedSpriteMetadata = AssetManager::GetAssetMetadata(component.Sprite);

			if (ImGui::BeginCombo(LABEL("Sprite").c_str(), selectedSpriteMetadata.Path.stem().string().c_str()))
			{
				Array<AssetMetadata> spritesMetadata = AssetManager::GetAssetMetadataList<Sprite>();
				for (const AssetMetadata& metadata : spritesMetadata)
				{
					if (ImGui::Selectable(metadata.Path.stem().string().c_str(), metadata.Handle == component.Sprite))
					{
						component.Sprite = metadata.Handle;
					}
				}
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
	
	void DetailsPanel::RenderBoxRendererComponent()
	{
		const char* id = "Box Renderer Component";
		bool opened = ImGui::CollapsingHeader(id, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
		RenderComponent_ContextMenu(id, Reflection::GetClass<BoxRendererComponent>());

		if (opened)
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
	
	void DetailsPanel::RenderLineRendererComponent()
	{
		const char* id = "Line Renderer Component";
		bool opened = ImGui::CollapsingHeader(id, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
		RenderComponent_ContextMenu(id, Reflection::GetClass<LineRendererComponent>());
		
		if (opened)
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
	
	void DetailsPanel::RenderRigidbody2DComponent()
	{
		const char* id = "Rigidbody2D Component";
		bool opened = ImGui::CollapsingHeader(id, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
		RenderComponent_ContextMenu(id, Reflection::GetClass<Rigidbody2DComponent>());
		
		if (opened)
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
	
	void DetailsPanel::RenderBoxCollider2DComponent()
	{
		const char* id = "BoxCollider2D Component";
		bool opened = ImGui::CollapsingHeader(id, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
		RenderComponent_ContextMenu(id, Reflection::GetClass<BoxCollider2DComponent>());

		if (opened)
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
	
	void DetailsPanel::RenderCircleCollider2DComponent()
	{
		const char* id = "CircleCollider2D Component";
		bool opened = ImGui::CollapsingHeader(id, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
		RenderComponent_ContextMenu(id, Reflection::GetClass<CircleCollider2DComponent>());
		
		if (opened)
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
	
	void DetailsPanel::RenderCapsuleCollider2DComponent()
	{
		const char* id = "CapsuleCollider2D Component";
		bool opened = ImGui::CollapsingHeader(id, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);
		RenderComponent_ContextMenu(id, Reflection::GetClass<CapsuleCollider2DComponent>());

		if (opened)
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

	void DetailsPanel::RenderComponent_ContextMenu(const char* label, const Reflection::Class& reflectionClass)
	{
		ImGui::SameLine();

		const char* label_end = ImGui::FindRenderedTextEnd(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, label_end, false);
		const ImVec2 padding = ImGui::GetStyle().FramePadding;
		const float frame_height = ImMax(ImMin(ImGui::GetCurrentWindow()->DC.CurrLineSize.y, ImGui::GetFontSize()), label_size.y);
		ImVec2 buttonSize{ frame_height, frame_height };

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - buttonSize.x - padding.x);

		ImGui::PushID(label);
		bool clicked = ImGui::ImageButton(label, (ImTextureID)Resources::ThreeDotsTexture->GetNativeTexture(), buttonSize);
		ImGui::PopID();

		if (clicked)
		{
			ImGui::OpenPopup(label);
		}

		if (ImGui::BeginPopup(label))
		{
			if (ImGui::MenuItem("Delete"))
			{
				m_ComponetTypeToRemove = &reflectionClass;
			}
			ImGui::EndPopup();
		}
	}
}