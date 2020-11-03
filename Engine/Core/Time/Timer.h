#ifndef INCLUDE_TIMER
#define INCLUDE_TIMER

namespace GameEngine
{
	class Timer
	{
	public:
		static float Time ();
		static float DeltaTime ();
		static float FixedDeltaTime ();
	};
}

#endif