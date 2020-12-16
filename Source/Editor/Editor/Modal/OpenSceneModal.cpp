#include "OpenSceneModal.h"
#include "Editor/Editor/Editor.h"
#include "Editor/Core/EditorGUI.h"
#include "Engine/Core/File/FileSystem.h"
#include "Engine/Engine/Scene/SceneManager.h"

namespace GameEngine
{
    OpenSceneModal::OpenSceneModal () : EditorModalWindow ("Open Scene"),
        m_scenePath ("")
    {
    }

    OpenSceneModal::~OpenSceneModal ()
    {
    }

    void OpenSceneModal::OnRender ()
    {
        EditorGUI::Label ("Scene Path");

        EditorGUI::Label ("Assets/");
        EditorGUI::SameLine ();
        m_scenePath = EditorGUI::InputText ("##path", m_scenePath);

        if (EditorGUI::Button ("Open"))
        {
            Editor::GetInstance ().SetEditingGameObject (nullptr);
            SceneManager::GetInstance ().LoadScene (PATH("Assets/") + PathString (m_scenePath.begin (), m_scenePath.end ()));
            Close ();
        }

        EditorGUI::SameLine ();

        if (EditorGUI::Button ("Cancel"))
        {
            Close ();
        }
    }
}
