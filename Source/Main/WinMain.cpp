#include <Windows.h>

#include "Editor/Editor/Editor.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nShowCmd)
{
	if (GameEngine::Editor::GetInstance ().Initialize () == true)
	{
		GameEngine::Editor::GetInstance ().Run ();
	}

	GameEngine::Editor::GetInstance ().Shutdown ();

	return 0;
}
