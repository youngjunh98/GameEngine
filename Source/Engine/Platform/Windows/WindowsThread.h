#pragma once

#include <Windows.h>
#include <process.h>

#include "Engine/Platform/Generic/GenericThread.h"

namespace GameEngine
{
	namespace Platform
	{
		class ENGINE_PLATFORM_API WindowsThread : public GenericThread
		{
		public:
			WindowsThread (ThreadFunction function, void* data);
			virtual ~WindowsThread ();

			virtual void Join () override;

			friend unsigned int WINAPI WindowsThreadFunction (void* data);

		protected:
			HANDLE m_handle;
		};
	}
}