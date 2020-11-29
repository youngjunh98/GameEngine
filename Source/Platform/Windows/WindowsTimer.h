#pragma once

#include <Windows.h>

#include "Platform/Generic/GenericTimer.h"

namespace GameEngine
{
	namespace Platform
	{
		class PLATFORM_API WindowsTimer : public GenericTimer
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
}
