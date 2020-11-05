#pragma once

#if defined(WIN32) | defined(_WIN32) | defined(WIN64) | defined(_WIN64)
	#define PLATFORM_EXPORT __declspec(dllexport)
	#define PLATFORM_IMPORT __declspec(dllimport)
#endif

#if defined(EXPORT_PLATFORM)
	#define PLATFORM_API PLATFORM_EXPORT
#elif defined(IMPORT_PLATFORM)
	#define PLATFORM_API PLATFORM_IMPORT
#else
	#define PLATFORM_API
#endif
