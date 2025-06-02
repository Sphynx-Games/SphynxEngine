#pragma once

#include "PropertyDrawer/PropertyDrawer.h"
#include <Math/Transform.h>


namespace Sphynx
{
	class TransformPropertyDrawer : public PropertyDrawer<TransformPropertyDrawer, Transform>
	{
	private:
		virtual void Draw(const Reflection::Property& property, void* data) override final;

	};
}
