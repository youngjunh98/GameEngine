#pragma once

#include "Engine/Platform/Generic/GenericThread.h"

namespace GameEngine
{
	namespace Platform
	{
		class ENGINE_PLATFORM_API MacThread : public GenericThread
		{
		public:
			MacThread (ThreadFunction function, void* data);
			virtual ~MacThread ();

			virtual void Join () override;
		};
	}
}
