#include "WindowMenu.h"
#include "Editor/Editor/Editor.h"
#include "Editor/Editor/Window/GameWindow.h"
#include "Editor/Editor/Window/InspectorWindow.h"
#include "Editor/Editor/Window/ProjectWindow.h"
#include "Editor/Editor/Window/SceneWindow.h"
#include "Editor/Editor/Window/ViewportWindow.h"

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

        if (MenuItem ("Viewport"))
        {
            Editor::GetInstance ().OpenWindow<ViewportWindow> ();
        }
	}
}
