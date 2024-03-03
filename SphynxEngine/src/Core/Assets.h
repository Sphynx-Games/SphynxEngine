#pragma once

#include "Renderer/Texture.h"
#include <unordered_map>
#include <string>

namespace Sphynx
{
	class Assets
	{
	public:
		/*inline static void Init()
		{
			LoadTextures();
		}

		inline static void Shutdown()
		{
			DeleteTextures();
		}*/

		//----------------//
		inline static Texture* AddTexture(const std::string& path)
		{
			m_Textures[path] = Texture::Create(path);
			return m_Textures[path];
		}

		/*inline static void AddTextures(const std::vector<const std::string&>& paths)
		{
			for (const std::string& path : paths)
			{
				AddTexture(path);
			}
		}*/

		inline static Texture* GetTexture(const std::string& path)
		{
			return m_Textures[path];
		}

	private:
		/*inline static void LoadTextures()
		{
			for (auto& it : m_Textures)
			{
				if (it.second == nullptr)
				{
					it.second = Texture::Create(it.first);
				}
			}
		}*/

		inline static void DeleteTextures()
		{
			for (auto& it : m_Textures)
			{
				delete it.second;
			}
			m_Textures.clear();
		}

	private:
		inline static std::unordered_map<std::string, Texture*> m_Textures;

		friend class Renderer2D;
	};
}