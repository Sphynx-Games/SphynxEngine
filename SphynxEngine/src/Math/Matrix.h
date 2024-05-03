#pragma once

#include "Renderer/Renderer2D.h"
#include "Transform.h"
#include <glm/ext/matrix_transform.hpp>

#include <glm/glm.hpp>

namespace Sphynx
{
	inline const glm::mat4 GetModelMatrixFromTransform(const Transform& transform)
	{
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), { transform.Position.X, transform.Position.Y, transform.Position.Z });

		glm::mat4 rotationMatrix = 
			glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation.X), glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation.Y), glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation.Z), glm::vec3(0, 0, 1));

		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(transform.Scale.X, transform.Scale.Y, transform.Scale.Z));

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

	inline const glm::mat4 GetMVPMatrix(const Transform& transform)
	{
		return Renderer2D::GetConfiguration().ViewProjectionMatrix * GetModelMatrixFromTransform(transform);
	}
}