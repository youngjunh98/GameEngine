#include "Platform/Generic/GenericPlatformThread.h"

namespace GameEngine
{
	GenericPlatformThread::GenericPlatformThread (ThreadFunction function, void* data) :
		m_function (function),
		m_data (data)
	{
	}

	GenericPlatformThread::~GenericPlatformThread ()
	{
	}
}