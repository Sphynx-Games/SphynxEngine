#pragma once

#include "Vector.h"

namespace Sphynx
{
	struct SPHYNX_API Transform
	{
		Transform() : 
			Position({ 0.0f, 0.0f, 0.0f }), 
			Scale({ 1.0f, 1.0f, 1.0f }),
			Rotation({ 0.0f, 0.0f, 0.0f })
		{
		}

		Transform(Vector3f position, Vector3f scale, Vector3f rotation) : 
			Position(position), 
			Scale(scale), 
			Rotation(rotation)
		{
		}

		Vector3f Position;
		Vector3f Scale;
		Vector3f Rotation;
	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Transform, SPHYNX_API)

SPX_REFLECT_PROPERTY(Position)
SPX_REFLECT_PROPERTY(Scale)
SPX_REFLECT_PROPERTY(Rotation)

SPX_REFLECT_STRUCT_END(Sphynx::Transform, SPHYNX_API)