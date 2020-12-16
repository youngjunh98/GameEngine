#include "Engine/Platform/Generic/GenericThread.h"

namespace GameEngine
{
	namespace Platform
	{
		GenericThread::GenericThread (ThreadFunction function, void* data) :
			m_function (function), m_data (data)
		{
		}

		GenericThread::~GenericThread ()
		{
		}
	}
}