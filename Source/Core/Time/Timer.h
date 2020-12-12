#pragma once

#include "Core/CoreMacro.h"

namespace GameEngine
{
	class CORE_API Timer
	{
	public:
		static float Time ();
		static float DeltaTime ();
		static float FixedDeltaTime ();
	};
}
