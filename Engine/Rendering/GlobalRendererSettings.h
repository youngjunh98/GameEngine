#ifndef INCLUDE_GLOBAL_RENDERER_SETTINGS
#define INCLUDE_GLOBAL_RENDERER_SETTINGS

#include "EngineType.h"

namespace GameEngine
{
	struct GlobalRendererSettings
	{
		uint32 m_renderWidth;
		uint32 m_renderHeight;
		bool m_bFullScreenEnabled;
		uint32 m_msaaCount;
		bool m_bMsaaEnabled;
		uint32 m_refreshRate;
		bool m_bVSyncEnabled;
	};
}

#endif