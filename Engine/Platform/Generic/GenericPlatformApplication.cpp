#include "Platform/Generic/GenericPlatformApplication.h"

namespace GameEngine
{
	void GenericPlatformApplication::RequestExit ()
	{
		m_bKeepRunning = false;
	}
}
