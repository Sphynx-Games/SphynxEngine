#pragma once

#include "Core/Core.h"
#include <filesystem>


#define PROJECT_EXTENSION ".spxproject"

namespace Sphynx
{
	struct ProjectInfo
	{
		std::string ProjectName;
		std::filesystem::path InitialScene;
	};
}


#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::ProjectInfo)

SPX_REFLECT_PROPERTY(ProjectName)
SPX_REFLECT_PROPERTY(InitialScene)

SPX_REFLECT_STRUCT_END(Sphynx::ProjectInfo)