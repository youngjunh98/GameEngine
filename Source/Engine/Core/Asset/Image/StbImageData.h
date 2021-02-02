#pragma once

#include "Type.h"
#include "Engine/Core/CoreMacro.h"

namespace GameEngine
{
	struct ENGINE_CORE_API StbImageData
	{
		StbImageData ();
		~StbImageData ();

		// Pointer to Image Data
		void* m_data;
	};
}
