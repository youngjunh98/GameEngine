#pragma once

#if defined(WIN32) | defined(_WIN32) | defined(WIN64) | defined(_WIN64)
	#define ENGINE_PLATFORM_WINDOWS
	
	#define ENGINE_PLATFORM_EXPORT __declspec(dllexport)
	#define ENGINE_PLATFORM_IMPORT __declspec(dllimport)
#endif

#if defined(EXPORT_ENGINE_PLATFORM)
	#define ENGINE_PLATFORM_API ENGINE_PLATFORM_EXPORT
#elif defined(IMPORT_ENGINE_PLATFORM)
	#define ENGINE_PLATFORM_API ENGINE_PLATFORM_IMPORT
#else
	#define ENGINE_PLATFORM_API
#endif
