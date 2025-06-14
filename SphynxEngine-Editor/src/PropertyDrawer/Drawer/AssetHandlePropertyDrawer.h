#pragma once

#include "PropertyDrawer/PropertyDrawer.h"
#include <Asset/Asset.h>


namespace Sphynx
{
	class AssetHandlePropertyDrawer : public PropertyDrawer<AssetHandlePropertyDrawer, AssetHandle>
	{
	private:
		virtual void Draw(const Reflection::Property& property, void* data) override final;

	};
}
