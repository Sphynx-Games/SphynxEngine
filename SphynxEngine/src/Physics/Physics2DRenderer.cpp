#include "Physics2DRenderer.h"
#include "Collider2D.h"
#include "Renderer/Renderer2D.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtc/quaternion.hpp"

#include <iostream>


namespace Sphynx
{
	extern const glm::mat4 GetModelMatrixFromTransform(const Transform& transform);

	Transform GetTransformWithOffset(const Transform& local, const Transform& transform)
	{
		glm::mat4 localMatrix = GetModelMatrixFromTransform(local);
		glm::mat4 worldMatrix = GetModelMatrixFromTransform(transform);
		glm::mat4 entityWorldMatrix = worldMatrix * localMatrix;

		glm::vec3 scale;
		glm::quat orientation;
		glm::vec3 pos;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(entityWorldMatrix, scale, orientation, pos, skew, perspective);

		glm::vec3 rotation = glm::eulerAngles(orientation);

		Transform entityTransform;
		entityTransform.Position = { pos.x, pos.y, pos.z };
		entityTransform.Rotation = { glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z) };
		entityTransform.Scale = { scale.x, scale.y, scale.z };
		return entityTransform;
	}

	void Physics2DRenderer::DrawBoxCollider(BoxCollider2D* collider, const Transform& transform)
	{
		if (collider == nullptr || !collider->NeedsDebug()) return;

		Transform local = {
					{ collider->GetOffset().X, collider->GetOffset().Y, 0.0f },
					{ collider->GetSize().X, collider->GetSize().Y, 1.0f },
					{ 0.0f, 0.0f, 0.0f }
		};
		Transform entityTransform = GetTransformWithOffset(local, transform);

		Renderer2D::DrawQuad(entityTransform);
	}

	void Physics2DRenderer::DrawCircleCollider(CircleCollider2D* collider, const Transform& transform)
	{
		if (collider == nullptr || !collider->NeedsDebug()) return;

		Transform local = {
					{ collider->GetOffset().X, collider->GetOffset().Y, 0.0f },
					{ collider->GetRadius() * 2.0f, collider->GetRadius() * 2.0f, 1.0f },
					{ 0.0f, 0.0f, 0.0f }
		};
		Transform entityTransform = GetTransformWithOffset(local, transform);
		float maxScale = std::max(entityTransform.Scale.X, entityTransform.Scale.Y);
		entityTransform.Scale.X = maxScale;
		entityTransform.Scale.Y = maxScale;

		Renderer2D::DrawCircle(entityTransform);
	}

	void Physics2DRenderer::DrawCapsuleCollider(CapsuleCollider2D* collider, const Transform& transform)
	{
		if (collider == nullptr || !collider->NeedsDebug()) return;

		// draw each fixture
		// -- BOX
		Vector2f capsuleSize = collider->GetSize() * Vector2f{ transform.Scale.X, transform.Scale.Y };
		Vector2f boxSize =
		{
			(capsuleSize.X > capsuleSize.Y) ? capsuleSize.X - capsuleSize.Y : capsuleSize.X,
			(capsuleSize.X > capsuleSize.Y) ? capsuleSize.Y : capsuleSize.Y - capsuleSize.X
		};
		boxSize /= Vector2f{ transform.Scale.X, transform.Scale.Y };

		Transform local = {
				{ collider->GetOffset().X, collider->GetOffset().Y, 0.0f },
				{ 1.0f, 1.0f, 1.0f },
				{ 0.0f, 0.0f, 0.0f }
		};
		Transform entityTransform = GetTransformWithOffset(local, transform);

		Renderer2D::DrawQuad(entityTransform, boxSize);

		// bounding box
		//Renderer2D::DrawQuad(entityTransform, collider->Size, { 0.5, 0.5f }, Color::Green);

		// -- CIRCLES
		// right, up
		Transform circleTransformA;
		circleTransformA.Position = (capsuleSize.X > capsuleSize.Y)
			? Vector3f{ (boxSize.X / 2.0f) + collider->GetOffset().X, collider->GetOffset().Y , 0.0f }
		    : Vector3f{ collider->GetOffset().X, (boxSize.Y / 2.0f) + collider->GetOffset().Y, 0.0f };
		circleTransformA.Scale /= transform.Scale;
		circleTransformA = GetTransformWithOffset(circleTransformA, transform);
		float circleRadiusA = std::min(capsuleSize.X, capsuleSize.Y) / 2.0f;

		// left, down
		Transform circleTransformB;
		circleTransformB.Position = (capsuleSize.X > capsuleSize.Y)
			? Vector3f{ (-boxSize.X / 2.0f) + collider->GetOffset().X, collider->GetOffset().Y, 0.0f }
		    : Vector3f{ collider->GetOffset().X, (-boxSize.Y / 2.0f) + collider->GetOffset().Y, 0.0f };
		circleTransformB.Scale /= transform.Scale;
		circleTransformB = GetTransformWithOffset(circleTransformB, transform);
		float circleRadiusB = std::min(capsuleSize.X, capsuleSize.Y) / 2.0f;

		Renderer2D::DrawCircle(circleTransformA, circleRadiusA);
		Renderer2D::DrawCircle(circleTransformB, circleRadiusB);
	}
}