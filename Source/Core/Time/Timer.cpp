#include "Core/Time/Timer.h"
#include "Platform/Platform.h"

namespace GameEngine
{
	float Timer::Time ()
	{
		return Platform::GetGenericTimer ().GetTime ();
	}

	float Timer::DeltaTime ()
	{
		return Platform::GetGenericTimer ().GetDeltaTime ();
	}

	float Timer::FixedDeltaTime ()
	{
		return Platform::GetGenericTimer ().GetFixedDeltaTime ();
	}
}