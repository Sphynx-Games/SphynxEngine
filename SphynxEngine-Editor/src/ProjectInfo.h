#pragma once

#include "Core/Core.h"
#include <filesystem>


#define PROJECT_EXTENSION ".spxproject"

namespace Sphynx
{
	struct ProjectInfo
	{
		std::string Name;
		std::filesystem::path InitialScene;
	};
}


#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::ProjectInfo) // TODO: add API macro

SPX_REFLECT_PROPERTY(Name)
SPX_REFLECT_PROPERTY(InitialScene)

SPX_REFLECT_STRUCT_END(Sphynx::ProjectInfo)