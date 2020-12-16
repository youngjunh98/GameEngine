#ifndef INCLUDE_APPLICATION
#define INCLUDE_APPLICATION

#include "Type.h"
#include "Engine/Core/CoreMacro.h"

namespace GameEngine
{
	class ENGINE_CORE_API Application
	{
	public:
		static void ResizeWindow (uint32 width, uint32 height, bool bFullScreen);
		static void Quit ();
	};
}

#endif