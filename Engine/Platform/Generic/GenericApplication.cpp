#include "Platform/Generic/GenericApplication.h"

namespace GameEngine
{
	namespace Platform
	{
		void GenericApplication::RequestExit ()
		{
			m_bKeepRunning = false;
		}
	}
}
