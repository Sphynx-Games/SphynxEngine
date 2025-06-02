#pragma once

#include "PropertyDrawer/PropertyDrawer.h"
#include <Math/Color.h>


namespace Sphynx
{
	class ColorPropertyDrawer : public PropertyDrawer<ColorPropertyDrawer, Color>
	{
	private:
		virtual void Draw(const Reflection::Property& property, void* data) override final;

	};
}
