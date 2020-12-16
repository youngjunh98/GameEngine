#include "FileMenu.h"
#include "Editor/Editor/Editor.h"
#include "Editor/Editor/Modal/CreateSceneModal.h"
#include "Editor/Editor/Modal/OpenSceneModal.h"
#include "Engine/Engine/Scene/SceneManager.h"

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
            SceneManager::GetInstance ().SaveScene ();
        }
	}
}
