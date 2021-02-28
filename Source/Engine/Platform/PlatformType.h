#pragma once

#include "PlatformMacro.h"

namespace GameEngine
{
	namespace Platform
	{
		#if defined(ENGINE_PLATFORM_WINDOWS)
			// Windows Types
			typedef unsigned char byte;

			typedef signed char int8;
			typedef signed short int16;
			typedef signed int int32;
			typedef signed long long int64;

			typedef unsigned char uint8;
			typedef unsigned short uint16;
			typedef unsigned int uint32;
			typedef unsigned long long uint64;

			#if defined(UNICODE) | defined(_UNICODE)
				typedef wchar_t path_char;
			#else
				typedef char path_char;
			#endif
		#elif defined(ENGINE_PLATFORM_MAC)
            // Mac Types
            typedef unsigned char byte;
    
            typedef signed char int8;
            typedef signed short int16;
            typedef signed int int32;
            typedef signed long long int64;

            typedef unsigned char uint8;
            typedef unsigned short uint16;
            typedef unsigned int uint32;
            typedef unsigned long long uint64;
    
            typedef char path_char;
        #else
			#error("Missing platform type!")
		#endif
		
		static_assert (sizeof (byte) == 1, "size test of byte is failed!");

		static_assert (sizeof (int8) == 1, "size test of int8 is failed!");
		static_assert (sizeof (int16) == 2, "size test of int16 is failed!");
		static_assert (sizeof (int32) == 4, "size test of int32 is failed!");
		static_assert (sizeof (int64) == 8, "size test of int64 is failed!");

		static_assert (sizeof (uint8) == 1, "size test of uint8 is failed!");
		static_assert (sizeof (uint16) == 2, "size test of uint16 is failed!");
		static_assert (sizeof (uint32) == 4, "size test of uint32 is failed!");
		static_assert (sizeof (uint64) == 8, "size test of uint64 is failed!");
	}
}
