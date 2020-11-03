#ifndef INCLUDE_PLATFORM_RENDERING_INTERFACE
#define INCLUDE_PLATFORM_RENDERING_INTERFACE

#if defined (PLATFORM_WINDOWS) && defined (RI_D3D11)

	#include "RI/D3D11/D3D11RenderingInterface.h"
	#include "RI/D3D11/D3D11RenderingInterfaceResource.h"
	#include "RI/D3D11/D3D11RenderingInterfaceEnum.h"

	namespace GameEngine
	{
		using PlatformRenderingInterface = D3D11RenderingInterface;
	}

	#else
		#error ("Missing platform rendering interface!")
	#endif

#endif