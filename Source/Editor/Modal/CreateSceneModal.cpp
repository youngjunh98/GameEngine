#include "CreateSceneModal.h"
#include "Editor/Editor.h"
#include "Editor/EditorGUI.h"
#include "Core/File/FileSystem.h"
#include "Engine/Scene/SceneManager.h"

namespace GameEngine
{
    CreateSceneModal::CreateSceneModal () : EditorModalWindow ("Create Scene"),
        m_scenePath ("")
    {
    }

    CreateSceneModal::~CreateSceneModal ()
    {
    }

    void CreateSceneModal::OnRender ()
    {
        EditorGUI::Label ("Scene Path");

        EditorGUI::Label ("Assets/");
        EditorGUI::SameLine ();
        m_scenePath = EditorGUI::InputText ("##path", m_scenePath);

        if (EditorGUI::Button ("Open"))
        {
            Editor::GetInstance ().SetEditingGameObject (nullptr);
            g_sceneManager.CreateEmptyScene (PATH("Assets/") + PathString (m_scenePath.begin (), m_scenePath.end ()));
            Close ();
        }

        EditorGUI::SameLine ();

        if (EditorGUI::Button ("Cancel"))
        {
            Close ();
        }
    }
}
