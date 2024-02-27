#pragma once

#include <SDL3/SDL.h>
#include <glm/ext/matrix_transform.hpp>

namespace Sphynx
{
	inline void ChangeToSphynxCoords(Vector2i& point, const Window* window) 
	{
		point.Y = window->GetHeight() - point.Y;
	}

	inline const glm::mat4 MultTransformMatrices(const Transform& transform)
	{
		glm::mat4 matTrans = glm::translate(glm::mat4(1.0f), { transform.Position.X, -transform.Position.Y, transform.Position.Z });

		glm::mat4 matRotX = glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation.X), glm::vec3(1, 0, 0));
		glm::mat4 matRotY = glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation.Y), glm::vec3(0, 1, 0));
		glm::mat4 matRotZ = glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation.Z), glm::vec3(0, 0, 1));

		glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(transform.Scale.X, transform.Scale.Y, transform.Scale.Z));

		return matTrans * matRotX * matRotY * matRotZ * matScale;
	}

	inline void DrawPoligon(
		SDL_Renderer* renderer,
		DrawMode drawMode,
		const std::vector<SDL_FPoint>& points,
		const std::vector<int32_t>& indices,
		Color color,
		bool isCircle = false
	)
	{
		switch (drawMode) {
		case DrawMode::WIREFRAME:
		{
			SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
			SDL_RenderLines(
				renderer,
				(isCircle) ? points.data() + 1 : points.data(),
				(isCircle) ? points.size() - 1 : points.size()
			);
		}
		break;

		case DrawMode::FILLED:
		{
			SDL_Color sdlColor = { color.R, color.G, color.B, color.A };
			SDL_RenderGeometryRaw(
				renderer,
				nullptr,
				(float*)points.data(), sizeof(float) * 2,
				&sdlColor, 0,
				nullptr, 0,
				(isCircle) ? points.size() - 1 : points.size(),
				indices.data(), indices.size(), sizeof(int32_t)
			);
		}
		break;
		}
	}
}