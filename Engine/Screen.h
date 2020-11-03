#ifndef INCLUDE_SCREEN
#define INCLUDE_SCREEN

#include "Core/CoreType.h"

namespace GameEngine
{
	class Screen
	{
	public:
		Screen ();
		virtual ~Screen () = 0;

		static void ChangeResolution (uint32 width, uint32 height, bool bFullScreenEnabled, uint32 refreshRate);

		static uint32 Width ();
		static uint32 Height ();
		static float AspectRatio ();
	};
}

#endif