#pragma once

#include "Engine/Core/CoreMacro.h"

namespace GameEngine
{
	class ENGINE_CORE_API Timer
	{
	public:
		static float Time ();
		static float DeltaTime ();
		static float FixedDeltaTime ();
	};
}
