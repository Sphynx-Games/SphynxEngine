#pragma once

#include <filesystem>

#define SANDBOX_API __declspec(dllexport)


const std::filesystem::path& PATH_INITIAL_SCENE = "Assets\\Scenes\\TestScene.spxasset";

extern "C" SANDBOX_API const std::filesystem::path& GetPathInitialScene()
{
	return PATH_INITIAL_SCENE;
}