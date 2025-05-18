#pragma once

#include "Core/Core.h"
#include "Reflection/ReflectionSerializer.h"
#include "Scene/Actor.h"


namespace Sphynx
{
	class SPHYNX_API SceneSerializer
	{
	public:
		SceneSerializer(const class Scene& scene, class Writer&& writer);

		void Serialize();

	private:
		void SerializeComponent(const Reflection::Class& componentClass, const Actor& actor);

	private:
		const Scene& m_Scene;
		Writer& m_Writer;
	};
}