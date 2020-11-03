#ifndef INCLUDE_APPLICATION
#define INCLUDE_APPLICATION

#include "Core/CoreType.h"
#include "Core/CoreMacro.h"

namespace GameEngine
{
	class CORE_API Application
	{
	public:
		static void ResizeWindow (uint32 width, uint32 height, bool bFullScreen);
		static void RequestExit ();
	};
}

#endif