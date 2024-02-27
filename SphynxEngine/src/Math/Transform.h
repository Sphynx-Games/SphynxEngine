#pragma once

#include "Vector.h"

namespace Sphynx
{
	struct Transform
	{
		Transform(Vector3f position, Vector3f scale, Vector3f rotation) 
			: Position(position), Scale(scale), Rotation(rotation)
		{
		}

		Vector3f Position;
		Vector3f Scale;
		Vector3f Rotation;
	};
}