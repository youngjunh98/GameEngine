#pragma once

#include <Windows.h>
#include <process.h>

#include "Platform/Generic/GenericThread.h"

namespace GameEngine
{
	namespace Platform
	{
		class PLATFORM_API WindowsThread : public GenericThread
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