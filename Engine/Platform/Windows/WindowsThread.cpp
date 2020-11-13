#include "Platform/Windows/WindowsThread.h"

namespace GameEngine
{
	namespace Platform
	{
		unsigned int WINAPI WindowsThreadFunction (void* data)
		{
			auto* thread = reinterpret_cast<WindowsThread*> (data);
			int32 threadId = static_cast<int32> (GetThreadId (thread));
			int32 result = thread->m_function (thread, threadId, thread->m_data);

			return static_cast<unsigned int> (result);
		}

		WindowsThread::WindowsThread (ThreadFunction function, void* data) : GenericThread (function, data),
			m_handle (nullptr)
		{
			m_handle = reinterpret_cast<HANDLE> (_beginthreadex (nullptr, 0, &WindowsThreadFunction, this, 0, nullptr));
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
