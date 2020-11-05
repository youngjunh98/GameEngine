#include <Windows.h>

#include "Engine.h"
#include "Core/Modular/ModuleManager.h"
#include "RI/D3D11/D3D11RenderingInterface.h"

LINK_MODULE (D3D11RI, Engine_RI.dll, GameEngine::D3D11RenderingInterface)

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nShowCmd)
{
	if (GameEngine::g_engine.Init () == true)
	{
		GameEngine::g_engine.Run ();
	}

	GameEngine::g_engine.Shutdown ();

	return 0;
}
