#include "RenderingInterface.h"

namespace GameEngine
{
	RenderingInterface::RenderingInterface ()
	{
	}

	RenderingInterface::~RenderingInterface ()
	{
	}

	void RenderingInterface::Shutdown ()
	{
		m_resourceRefList.clear ();
	}
}
