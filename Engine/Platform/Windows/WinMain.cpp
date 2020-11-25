#include <Windows.h>

#include "Engine/Engine.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nShowCmd)
{
	if (GameEngine::g_engine.Init () == true)
	{
		GameEngine::g_engine.Run ();
	}

	GameEngine::g_engine.Shutdown ();

	return 0;
}
