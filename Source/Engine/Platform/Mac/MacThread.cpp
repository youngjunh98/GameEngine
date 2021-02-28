#include "MacThread.h"

namespace GameEngine
{
	namespace Platform
	{
		MacThread::MacThread (ThreadFunction function, void* data) : GenericThread (function, data)
		{
		}

		MacThread::~MacThread ()
		{
		}

		void MacThread::Join ()
		{
		}
	}
}
