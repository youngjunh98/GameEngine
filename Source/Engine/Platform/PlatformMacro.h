#pragma once

#if defined(WIN32) | defined(_WIN32) | defined(WIN64) | defined(_WIN64)
	#define ENGINE_PLATFORM_WINDOWS
	
	#define ENGINE_PLATFORM_EXPORT __declspec(dllexport)
	#define ENGINE_PLATFORM_IMPORT __declspec(dllimport)
#elif defined(__APPLE__)
    #include <TargetConditionals.h>

    #if defined(TARGET_OS_MAC)
        #define ENGINE_PLATFORM_MAC
    #elif defined(TARGET_OS_IOS)
        #define ENGINE_PLATFORM_IOS
    #elif defined(TARGET_OS_WATCH)
        #define ENGINE_PLATFORM_APPLE_WATCH
    #elif defined(TARGET_OS_TV)
        #define ENGINE_PLATFORM_APPLE_TV
    #else
        #warning("Unkown Apple platform!")
    #endif

    #define ENGINE_PLATFORM_EXPORT __attribute__((visibility("default"))
    #define ENGINE_PLATFORM_IMPORT __attribute__((weak_import))
#endif

#if defined(EXPORT_ENGINE_PLATFORM)
	#define ENGINE_PLATFORM_API ENGINE_PLATFORM_EXPORT
#elif defined(IMPORT_ENGINE_PLATFORM)
	#define ENGINE_PLATFORM_API ENGINE_PLATFORM_IMPORT
#else
	#define ENGINE_PLATFORM_API
#endif
