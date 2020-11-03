#pragma once

#include <Windows.h>

#include "Platform/Generic/GenericPlatformTimer.h"

namespace GameEngine
{
	class PLATFORM_API WindowsTimer : public GenericPlatformTimer
	{
	public:
		WindowsTimer ();
		virtual ~WindowsTimer ();

		virtual void Reset (float fixedDeltaTime) override;
		virtual void Tick () override;

		LARGE_INTEGER GetTick () const;

	private:
		float m_tickInterval;
		LARGE_INTEGER m_resetTick;
		LARGE_INTEGER m_previousTick;
	};
}
