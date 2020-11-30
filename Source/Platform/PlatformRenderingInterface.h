#pragma once

#include "PlatformMacro.h"

#if defined (PLATFORM_WINDOWS) && defined (RI_D3D11)

	#include "RI/D3D11/D3D11RenderingInterface.h"
	#include "RI/D3D11/D3D11RenderingInterfaceResource.h"
	#include "RI/D3D11/D3D11RenderingInterfaceEnum.h"

	namespace GameEngine
	{
		namespace Platform
		{
			using RenderingInterface = D3D11RenderingInterface;
		}
	}

#else
	#error ("Missing platform rendering interface!")
#endif