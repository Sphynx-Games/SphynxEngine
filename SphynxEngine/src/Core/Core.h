#pragma once


// TODO: Add other definitions for other platforms, for now we only support windows
#ifdef SPX_PLATFORM_WINDOWS
#if 0
	#ifdef SPX_BUILD_SHARED
		#define SPHYNX_API __declspec(dllexport)
	#else
		#define SPHYNX_API __declspec(dllimport)
	#endif
#else
	#define SPHYNX_API
#endif
#else
	#error Sphynx only supports Windows (for now)
#endif

#include "Logging/Log.h"
#define SPX_CORE_ASSERT(Condition, ...) do { if (!(Condition)) { /* SPX_CORE_LOG_ERROR(##__VA_ARGS__); */ __debugbreak(); } } while(false)
