#pragma once

#include <SDL3/SDL.h>
#include <glm/ext/matrix_transform.hpp>

// This is used in the world-space draw commands
// so that screen coordinates starts in the center of the screen
// to simulate correctly the cameras POV
#define SDL_COORDS_TO_SPHYNX_COORDS(point, window) \
{\
	float width = (float)window->GetWidth();\
	float height = (float)window->GetHeight();\
	float aspectRatio = width / height;\
	point.x *= width / 2.0f; \
	point.x /= aspectRatio;\
	point.y *= height / 2.0f;\
	point.x += width / 2.0f;\
	point.y = (height - point.y) - height / 2.0f;\
}

namespace Sphynx
{
	inline void ChangeToSphynxCoords(Vector2i& point, const Window* window)
	{
		point.Y = window->GetHeight() - point.Y;
	}

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

	inline void DrawPolygon(
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
				(isCircle) ? (int32_t)points.size() - 1 : (int32_t)points.size()
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
				(isCircle) ? (int32_t)points.size() - 1 : (int32_t)points.size(),
				indices.data(), (int32_t)indices.size(), sizeof(int32_t)
			);
		}
		break;
		}
	}
}