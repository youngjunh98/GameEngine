#pragma once

#include "PlatformMacro.h"

#if defined (PLATFORM_WINDOWS)

namespace GameEngine
{
	// Windows Type
	typedef signed char platform_int8;
	typedef signed short platform_int16;
	typedef signed int platform_int32;
	typedef signed long long platform_int64;

	typedef unsigned char platform_uint8;
	typedef unsigned short platform_uint16;
	typedef unsigned int platform_uint32;
	typedef unsigned long long platform_uint64;
}

#else
	#error ("Missing platform type!")
#endif