#pragma once

#include <Windows.h>

#include "Engine/Platform/Generic/GenericTimer.h"

namespace GameEngine
{
	namespace Platform
	{
		class ENGINE_PLATFORM_API MacTimer : public GenericTimer
		{
		public:
			MacTimer ();
			virtual ~MacTimer ();

			virtual void Reset (float fixedDeltaTime) override;
			virtual void Tick () override;
		};
	}
}
