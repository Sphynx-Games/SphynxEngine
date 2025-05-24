#pragma once


#ifndef SPX_PLATFORM_WINDOWS
	#error Sphynx only supports Windows (for now)
#endif // SPX_PLATFORM_WINDOWS


#ifdef SPX_PLATFORM_WINDOWS
	#define DLL_EXPORT __declspec(dllexport)
	#define DLL_IMPORT __declspec(dllimport)
#else
	#define DLL_EXPORT
	#define DLL_IMPORT
#endif // SPX_PLATFORM_WINDOWS

#ifdef SPX_BUILD_SHARED
	#ifdef SPHYNX_BUILD
		#define SPHYNX_API DLL_EXPORT
	#else
		#define SPHYNX_API DLL_IMPORT
	#endif // SPHYNX_BUILD
#else
	#define SPHYNX_API
#endif // SPX_BUILD_SHARED

#include "Logging/Log.h"
#define SPX_CORE_ASSERT(Condition, ...) do { if (!(Condition)) { /* SPX_CORE_LOG_ERROR(##__VA_ARGS__); */ __debugbreak(); } } while(false)
