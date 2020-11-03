#pragma once

#include "Core/CoreType.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	using ThreadFunction = int32 (*) (void* thread, int32 threadId, void* data);

	class PLATFORM_API GenericPlatformThread
	{
	public:
		GenericPlatformThread (ThreadFunction function, void* data);
		virtual ~GenericPlatformThread ();

		virtual void Join () = 0;

	protected:
		ThreadFunction m_function;
		void* m_data;
	};
}