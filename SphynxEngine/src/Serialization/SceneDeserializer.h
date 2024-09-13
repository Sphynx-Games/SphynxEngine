#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	class SPHYNX_API SceneDeserializer
	{
	public:
		SceneDeserializer(class Scene& scene, class Reader& reader);

		void Deserialize();

	private:
		Scene& m_Scene;
		Reader& m_Reader;
	};
}