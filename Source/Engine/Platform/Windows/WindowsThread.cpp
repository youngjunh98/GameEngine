#include "Engine/Platform/Windows/WindowsThread.h"

namespace GameEngine
{
	namespace Platform
	{
		unsigned WINAPI WindowsThreadFunction (void* data)
		{
			auto* thread = reinterpret_cast<WindowsThread*> (data);
			int32 threadId = static_cast<int32> (GetThreadId (thread->m_handle));
			int32 result = thread->m_function (thread, threadId, thread->m_data);
			
			return static_cast<unsigned int> (result);
		}

		WindowsThread::WindowsThread (ThreadFunction function, void* data) : GenericThread (function, data),
			m_handle (nullptr)
		{
			auto threadHandle = _beginthreadex (nullptr, 0, &WindowsThreadFunction, this, 0, nullptr);

			if (threadHandle != 0)
			{
				m_handle = reinterpret_cast<HANDLE> (threadHandle);
			}
		}

		WindowsThread::~WindowsThread ()
		{
			if (m_handle != nullptr)
			{
				CloseHandle (m_handle);
			}
		}

		void WindowsThread::Join ()
		{
			if (m_handle != nullptr)
			{
				WaitForSingleObject (m_handle, INFINITE);
			}
		}
	}
}
