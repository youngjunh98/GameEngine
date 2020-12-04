#include "WindowMenu.h"
#include "Editor/Editor.h"
#include "Editor/Window/GameWindow.h"
#include "Editor/Window/InspectorWindow.h"
#include "Editor/Window/ProjectWindow.h"
#include "Editor/Window/SceneWindow.h"

namespace GameEngine
{
	WindowMenu::WindowMenu () : EditorMenu ("Window")
	{
	}

	WindowMenu::~WindowMenu ()
	{
	}

	void WindowMenu::OnRender ()
	{
        if (MenuItem ("Game"))
        {
            Editor::GetInstance ().OpenWindow<GameWindow> ();
        }

        if (MenuItem ("Inspector"))
        {
            Editor::GetInstance ().OpenWindow<InspectorWindow> ();
        }

        if (MenuItem ("Project"))
        {
            Editor::GetInstance ().OpenWindow<ProjectWindow> ();
        }

        if (MenuItem ("Scene"))
        {
            Editor::GetInstance ().OpenWindow<SceneWindow> ();
        }
	}
}
