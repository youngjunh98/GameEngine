#include "SceneWindow.h"
#include "Editor.h"
#include "DearImGui.h"
#include "Engine/GameObject.h"
#include "Engine/Scene/SceneManager.h"

namespace GameEngine
{
    SceneWindow::SceneWindow () : EditorWindow ("Scene")
    {
    }

    SceneWindow::~SceneWindow ()
    {
    }

    void SceneWindow::OnRender ()
    {
        Scene* scene = g_sceneManager.GetScene ();

        if (scene == nullptr)
        {
            return;
        }

        Editor& editor = Editor::GetInstance ();
        GameObject* editingGameObject = editor.GetEditingGameObject ();

        if (ImGui::BeginPopupContextWindow ("edit-gameobject-popup"))
        {
            if (ImGui::Selectable ("Add GameObject"))
            {
                editingGameObject = scene->SpawnGameObject ();
                editor.SetEditingGameObject (editingGameObject);
            }

            if (ImGui::Selectable ("Delete GameObject"))
            {
                if (editingGameObject != nullptr)
                {
                    editingGameObject->Destroy ();
                    editingGameObject = nullptr;

                    editor.SetEditingGameObject (nullptr);
                }
            }

            ImGui::EndPopup ();
        }

        uint32 id = 0;

        for (auto& gameObjectPtr : scene->GetGameObjects ())
        {
            GameObject* gameObject = gameObjectPtr.get ();

            if (gameObject->IsAlive () == false)
            {
                continue;
            }

            std::string label = gameObject->GetName () + "##" + std::to_string (id);
            bool bSelected = gameObject == editingGameObject;

            if (ImGui::Selectable (label.c_str (), &bSelected, ImGuiSelectableFlags_None))
            {
                editingGameObject = gameObject;
                editor.SetEditingGameObject (editingGameObject);
            }

            id += 1;
        }
    }
}
