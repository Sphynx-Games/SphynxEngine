#pragma once

#include "PropertyDrawer/PropertyDrawer.h"
#include <Scene/Actor.h>


namespace Sphynx
{
	class ActorPropertyDrawer : public PropertyDrawer<ActorPropertyDrawer, Actor>
	{
	private:
		virtual void Draw(const Reflection::Property& property, void* data) override final;

	};
}
