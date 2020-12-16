#pragma once

#include "PlatformMacro.h"

#if defined (ENGINE_PLATFORM_WINDOWS) && defined (ENGINE_RI_D3D11)

	#include "Engine/RI/D3D11/D3D11RenderingInterface.h"
	#include "Engine/RI/D3D11/D3D11RenderingInterfaceResource.h"
	#include "Engine/RI/D3D11/D3D11RenderingInterfaceEnum.h"

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