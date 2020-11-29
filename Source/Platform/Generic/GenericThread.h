#pragma once

#include "Type.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	using ThreadFunction = int32 (*) (void* thread, int32 threadId, void* data);
	
	namespace Platform
	{
		class PLATFORM_API GenericThread
		{
		public:
			GenericThread (ThreadFunction function, void* data);
			virtual ~GenericThread ();

			virtual void Join () = 0;

		protected:
			ThreadFunction m_function;
			void* m_data;
		};
	}
}