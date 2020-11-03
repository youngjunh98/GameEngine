#pragma once

#include <Windows.h>
#include <process.h>

#include "Platform/Generic/GenericPlatformThread.h"

namespace GameEngine
{
	class PLATFORM_API WindowsThread : public GenericPlatformThread
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