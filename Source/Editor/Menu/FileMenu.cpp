#include "FileMenu.h"
#include "Editor/Editor.h"
#include "Editor/Modal/CreateSceneModal.h"
#include "Editor/Modal/OpenSceneModal.h"
#include "Engine/Scene/SceneManager.h"

namespace GameEngine
{
	FileMenu::FileMenu () : EditorMenu ("File")
	{
	}

	FileMenu::~FileMenu ()
	{
	}

	void FileMenu::OnRender ()
	{
        if (MenuItem ("Create Scene"))
        {
            Editor::GetInstance ().OpenModalWindow<CreateSceneModal> ();
        }

        if (MenuItem ("Open Scene"))
        {
            Editor::GetInstance ().OpenModalWindow<OpenSceneModal> ();
        }

        if (MenuItem ("Save Scene"))
        {
            g_sceneManager.SaveScene ();
        }
	}
}
