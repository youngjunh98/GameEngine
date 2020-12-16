#pragma once

#include "Type.h"

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
