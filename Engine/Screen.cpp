#include "Screen.h"
#include "Rendering/GlobalRenderer.h"
#include "Core/Application/Application.h"

namespace GameEngine
{
	Screen::Screen ()
	{
		// Do nothing
	}

	Screen::~Screen ()
	{
		// Do nothing
	}

	void Screen::ChangeResolution (uint32 width, uint32 height, bool bFullScreenEnabled, uint32 refreshRate)
	{
		Application::ResizeWindow (width, height, bFullScreenEnabled);
		g_renderer.ChangeScreenResolution (width, height, bFullScreenEnabled);
	}

	uint32 Screen::Width ()
	{
		return g_renderer.GetSettings ().m_renderWidth;
	}

	uint32 Screen::Height ()
	{
		return g_renderer.GetSettings ().m_renderHeight;
	}

	float Screen::AspectRatio ()
	{
		return static_cast<float> (Width ()) / static_cast<float> (Height ());
	}
}
