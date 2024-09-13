#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	class SPHYNX_API SceneSerializer
	{
	public:
		SceneSerializer(const class Scene& scene, class Writer& writer);

		void Serialize();

	private:
		const Scene& m_Scene;
		Writer& m_Writer;
	};
}