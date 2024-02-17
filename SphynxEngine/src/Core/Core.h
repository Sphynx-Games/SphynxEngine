#pragma once


// TODO: Add other definitions for other platforms, for now we only support windows
#ifdef SPX_PLATFORM_WINDOWS
	#ifdef SPX_BUILD_DLL
		#define SPHYNX_API __declspec(dllexport)
	#else
		#define SPHYNX_API __declspec(dllimport)
	#endif
#else
	#error Sphynx only supports Windows (for now)
#endif
