#pragma once

#include <filesystem>
#include "Renderer/Texture.h"


namespace Sphynx
{
	class Resources
	{
	public:
		static void Init()
		{
			FolderTexture = TextureLoader::Load(ResourcesPath / "folder.png");
			FileTexture = TextureLoader::Load(ResourcesPath / "file.png");
			SaveTexture = TextureLoader::Load(ResourcesPath / "save.png");
		}

		static void Shutdown()
		{
			delete FolderTexture;
			delete FileTexture;
			delete SaveTexture;
		}

	public:
		inline static const std::filesystem::path ResourcesPath = "SphynxEngineEditor\\Resources";

		inline static Texture* FolderTexture;
		inline static Texture* FileTexture;
		inline static Texture* SaveTexture;
	};
}