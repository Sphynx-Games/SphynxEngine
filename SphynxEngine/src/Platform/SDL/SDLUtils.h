#pragma once

#include <SDL3/SDL.h>
#include "Math/Color.h"

// This is used in the world-space draw commands
// so that screen coordinates starts in the center of the screen
// to simulate correctly the cameras POV
#define SDL_COORDS_TO_SPHYNX_COORDS(point, window) \
{\
	SDL_Rect rect;\
	SDL_GetRenderViewport(SDL_GetRenderer((SDL_Window*)window->GetNativeWindow()), &rect);\
	float width = rect.w;\
	float height = rect.h;\
	point.x *= width / 2.0f; \
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