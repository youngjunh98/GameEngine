#include "SceneWindow.h"
#include "Editor/Core/EditorGUI.h"
#include "Editor/Editor/Editor.h"
#include "Engine/Engine/GameObject.h"
#include "Engine/Engine/Scene/SceneManager.h"

namespace GameEngine
{
    SceneWindow::SceneWindow () : EditorWindow ("Scene"),
        m_hoveredGameObject (nullptr)
    {
    }

    SceneWindow::~SceneWindow ()
    {
    }

    void SceneWindow::OnRender ()
    {
        Scene* scene = SceneManager::GetInstance ().GetScene ();

        if (scene == nullptr)
        {
            return;
        }

        Editor& editor = Editor::GetInstance ();
        GameObject* editingGameObject = editor.GetEditingGameObject ();
        int32 id = 0;

        for (auto& gameObjectPtr : scene->GetGameObjects ())
        {
            GameObject* gameObject = gameObjectPtr.get ();

            if (gameObject->IsAlive () == false)
            {
                continue;
            }

            EditorGUI::PushID (id);
            id += 1;

            std::string label = gameObject->GetName ();
            bool bSelected = gameObject == editingGameObject;

            if (EditorGUI::SelectableLabel (label, bSelected))
            {
                editingGameObject = gameObject;
                editor.SetEditingGameObject (editingGameObject);
            }

            if (EditorGUI::IsLastItemHovered () && m_hoveredGameObject == nullptr)
            {
                m_hoveredGameObject = gameObject;
            }

            EditorGUI::PopID ();
        }

        if (EditorGUI::BeginPopupContext ("EditGameObjectPopup"))
        {
            if (EditorGUI::SelectableLabel ("Add GameObject", false))
            {
                editingGameObject = scene->SpawnGameObject ();
                editor.SetEditingGameObject (editingGameObject);
            }

            if (EditorGUI::SelectableLabel ("Delete GameObject", false, m_hoveredGameObject == nullptr))
            {
                if (m_hoveredGameObject != nullptr)
                {
                    if (editingGameObject == m_hoveredGameObject)
                    {
                        editor.SetEditingGameObject (nullptr);
                    }

                    m_hoveredGameObject->Destroy ();
                    m_hoveredGameObject = nullptr;
                }
            }

            EditorGUI::EndPopupContext ();
        }
        else
        {
            m_hoveredGameObject = nullptr;
        }
    }
}
