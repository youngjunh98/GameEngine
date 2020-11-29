#include "Platform/Windows/WindowsTimer.h"

namespace GameEngine
{
	namespace Platform
	{
		WindowsTimer::WindowsTimer () :
			m_tickInterval (0.0f), m_resetTick (), m_previousTick ()
		{
			LARGE_INTEGER ticksPerSecond;
			QueryPerformanceFrequency (&ticksPerSecond);

			m_tickInterval = 1.0f / static_cast<float> (ticksPerSecond.QuadPart);
		}

		WindowsTimer::~WindowsTimer ()
		{
		}

		void WindowsTimer::Reset (float fixedDeltaTime)
		{
			GenericTimer::Reset (fixedDeltaTime);

			m_resetTick = GetTick ();
			m_previousTick = m_resetTick;
		}

		void WindowsTimer::Tick ()
		{
			LARGE_INTEGER tick = GetTick ();

			float time = static_cast<float> (tick.QuadPart - m_resetTick.QuadPart) * m_tickInterval;
			float deltaTime = static_cast<float> (tick.QuadPart - m_previousTick.QuadPart) * m_tickInterval;

			// Update time and delta time of generic platform timer.
			UpdateInternal (time, deltaTime);

			// Update previous tick.
			m_previousTick = tick;
		}

		LARGE_INTEGER WindowsTimer::GetTick () const
		{
			LARGE_INTEGER tick;
			QueryPerformanceCounter (&tick);

			return tick;
		}
	}
}