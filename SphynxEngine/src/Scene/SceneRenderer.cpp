#include "SceneRenderer.h"

#include "Scene.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer2D.h"
#include "Asset/AssetManager.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Component/BoxRendererComponent.h"
#include "Component/LineRendererComponent.h"
#include "Component/Camera/CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


namespace Sphynx
{
	void SceneRenderer::Render(Scene& scene)
	{
		Camera camera;
		if (SceneRenderer::GetMainCamera(scene, camera))
		{
			SceneRenderer::Render(scene, &camera);
		}
		else
		{
			SceneRenderer::Render(scene, nullptr);
		}
	}

	void SceneRenderer::Render(Scene& scene, const Camera* camera)
	{
		Renderer2D::Begin(camera);

		// Draw every SPRITE RENDERER component in scene
		auto spriteGroup = scene.m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : spriteGroup)
		{
			auto [spriteRenderer, transform] = spriteGroup.get<SpriteRendererComponent, TransformComponent>(entity);
			if (spriteRenderer.Sprite != nullptr)
			{
				Renderer2D::DrawSprite(*spriteRenderer.Sprite, transform.Transform, spriteRenderer.Tint);
			}
			else
			{
				Renderer2D::DrawQuad(DrawMode::FILLED, transform.Transform, { 1.0f, 1.0f }, { 0.5f, 0.5f }, spriteRenderer.Tint);
			}
		}

		// Draw every BOX RENDERER component in scene
		auto boxGroup = scene.m_Registry.group<BoxRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : boxGroup)
		{
			auto [boxRenderer, transform] = boxGroup.get<BoxRendererComponent, TransformComponent>(entity);
			Renderer2D::DrawQuad(boxRenderer.DrawMode, transform.Transform, boxRenderer.Size, boxRenderer.Pivot, boxRenderer.Color);
		}

		// Draw every LINE RENDERER component in scene
		auto lineGroup = scene.m_Registry.group<LineRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : lineGroup)
		{
			auto [lineRenderer, transform] = lineGroup.get<LineRendererComponent, TransformComponent>(entity);
			Renderer2D::DrawLine(transform.Transform, lineRenderer.Point1, lineRenderer.Point2, lineRenderer.LineWidth, lineRenderer.Color);
		}

		Renderer2D::End();
	}

	bool SceneRenderer::GetMainCamera(Scene& scene, Camera& camera)
	{
		bool mainCameraFound = false;

		auto cameraGroup = scene.m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : cameraGroup)
		{
			auto [cameraComp, transformComp] = cameraGroup.get<CameraComponent, TransformComponent>(entity);

			// TODO: right now, only the first main camera found is rendered. change this in the future.
			if (!mainCameraFound && cameraComp.IsMainCamera)
			{
				mainCameraFound = true;

				Vector3f position = transformComp.Transform.Position;
				glm::mat4 transform =
					glm::translate(glm::mat4(1.0f), { position.X, position.Y, position.Z }) *
					glm::mat4_cast(glm::quat({ 0.0f, 0.0f, glm::radians(transformComp.Transform.Rotation.Z) }));

				Vector2i viewportSize = Renderer2D::GetViewportSize();
				glm::mat4 aspectRatioMatrix = glm::mat4(1.0f); // Identity matrix
				aspectRatioMatrix[0][0] = (float)viewportSize.Y / viewportSize.X;

				camera.ViewMatrix = glm::inverse(transform);
				camera.ProjectionMatrix = aspectRatioMatrix * cameraComp.GetProjectionMatrix();
				camera.ViewProjectionMatrix = camera.ProjectionMatrix * camera.ViewMatrix;
			}
		}
		return mainCameraFound;
	}
}
