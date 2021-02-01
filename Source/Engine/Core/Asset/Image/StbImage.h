#pragma once

#include "Type.h"
#include "Engine/Core/CoreMacro.h"
#include "Engine/RI/RenderingInterfaceEnum.h"

namespace GameEngine
{
	struct ENGINE_CORE_API StbImage
	{
		StbImage ();
		~StbImage ();

		// Pointer to Image Data
		void* m_data;
	};
}
