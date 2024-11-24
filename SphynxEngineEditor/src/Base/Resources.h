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
			PlayTexture = TextureLoader::Load(ResourcesPath / "play.png");
			StopTexture = TextureLoader::Load(ResourcesPath / "stop.png");
			PauseTexture = TextureLoader::Load(ResourcesPath / "pause.png");
		}

		static void Shutdown()
		{
			delete FolderTexture;
			delete FileTexture;
			delete SaveTexture;
			delete PlayTexture;
			delete StopTexture;
			delete PauseTexture;
		}

	public:
		inline static const std::filesystem::path ResourcesPath = "SphynxEngineEditor\\Resources";

		inline static Texture* FolderTexture;
		inline static Texture* FileTexture;
		inline static Texture* SaveTexture;
		inline static Texture* PlayTexture;
		inline static Texture* StopTexture;
		inline static Texture* PauseTexture;
	};
}