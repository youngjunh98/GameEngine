#include "Core/Application/Application.h"
#include "Platform/Platform.h"

namespace GameEngine
{
	void Application::ResizeWindow (uint32 width, uint32 height, bool bFullScreen)
	{
		Platform::GetGenericApplication ().ResizeWindow (width, height, bFullScreen);
	}

	void Application::Quit ()
	{
		Platform::GetGenericApplication ().RequestQuit ();
	}
}