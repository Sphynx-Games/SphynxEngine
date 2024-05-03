#pragma once

#include "Vector.h"

namespace Sphynx
{
	struct SPHYNX_API Transform
	{
		Transform()
			: Position({ 0.0f, 0.0f, 0.0f }), Scale({ 1.0f, 1.0f, 1.0f }), Rotation({ 0.0f, 0.0f, 0.0f })
		{
		}

		Transform(Vector3f position, Vector3f scale, Vector3f rotation) 
			: Position(position), Scale(scale), Rotation(rotation)
		{
		}

		Vector3f Position;
		Vector3f Scale;
		Vector3f Rotation;
	};
}