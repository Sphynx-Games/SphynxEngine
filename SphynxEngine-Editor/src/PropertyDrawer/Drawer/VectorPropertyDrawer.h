#pragma once

#include "PropertyDrawer/PropertyDrawer.h"
#include <Math/Vector.h>


namespace Sphynx
{
#define DEFINE_VECTOR_PROPERTY_DRAWERS() \
	X(Vector2i) \
	X(Vector2u) \
	X(Vector2f) \
	X(Vector2d) \
	X(Vector3i) \
	X(Vector3u) \
	X(Vector3f) \
	X(Vector3d) \
	X(Vector4i) \
	X(Vector4u) \
	X(Vector4f) \
	X(Vector4d)


#define X(_Type) \
	class _Type##PropertyDrawer : public PropertyDrawer<_Type##PropertyDrawer, _Type> \
	{ \
	private: \
		virtual void Draw(const Reflection::Property& property, void* data) override final; \
	};

	DEFINE_VECTOR_PROPERTY_DRAWERS()

#undef X
#undef DEFINE_VECTOR_PROPERTY_DRAWERS
}
