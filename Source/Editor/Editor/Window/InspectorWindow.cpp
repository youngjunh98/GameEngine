#include "InspectorWindow.h"
#include "Editor/Editor/Editor.h"
#include "Editor/Core/EditorGUI.h"
#include "Engine/Engine/GameObject.h"
#include "Engine/Engine/Component/Transform.h"
#include "Engine/Engine/Component/Camera.h"
#include "Engine/Engine/Component/Physics/Rigidbody.h"
#include "Engine/Engine/Component/Physics/BoxCollider.h"
#include "Engine/Engine/Component/Physics/SphereCollider.h"
#include "Engine/Engine/Component/Rendering/MeshRenderer.h"
#include "Engine/Engine/Component/Rendering/Light.h"

namespace GameEngine
{
    InspectorWindow::InspectorWindow () : EditorWindow ("Inspector")
    {
    }

    InspectorWindow::~InspectorWindow ()
    {
    }

    void InspectorWindow::OnRender ()
    {
        Editor& editor = Editor::GetInstance ();
        GameObject* editingGameObject = editor.GetEditingGameObject ();

		if (editingGameObject == nullptr)
		{
            return;
        }

        editingGameObject->SetActive (EditorGUI::InputCheckbox ("##active-checkbox", editingGameObject->IsActive ()));
        EditorGUI::SameLine ();
        editingGameObject->SetName (EditorGUI::InputText ("Name", editingGameObject->GetName ()));
        editingGameObject->SetTag (EditorGUI::InputText ("Tag", editingGameObject->GetTag ()));

        int32 id = 0;

        for (auto* component : editingGameObject->GetComponents<Component> ())
        {
            if (component->IsAlive () == false)
            {
                continue;
            }

            EditorGUI::PushID (id);
            id += 1;

            std::string componentName = component->GetName ();

            EditorGUI::Separator ();
            EditorGUI::Label (componentName);

            if (componentName != "Transform")
            {
                EditorGUI::SameLine ();

                if (EditorGUI::Button ("Edit"))
                {
                    EditorGUI::OpenPopup ("Edit Popup");
                }

                if (EditorGUI::BeginPopup ("Edit Popup"))
                {
                    if (EditorGUI::SelectableLabel ("Delete", false))
                    {
                        component->Destroy ();
                    }

                    EditorGUI::EndPopup ();
                }
            }

            component->OnRenderEditor ();

            EditorGUI::PopID ();
        }

        EditorGUI::Separator ();

        if (EditorGUI::Button ("Add Component"))
        {
            EditorGUI::OpenPopup ("Add Component Popup");
        }

        if (EditorGUI::BeginPopup ("Add Component Popup"))
        {
            if (EditorGUI::SelectableLabel ("Camera", false))
            {
                editingGameObject->AddComponent<Camera> ();
            }
            else if (EditorGUI::SelectableLabel ("Mesh Renderer", false))
            {
                editingGameObject->AddComponent<MeshRenderer> ();
            }
            else if (EditorGUI::SelectableLabel ("Light", false))
            {
                editingGameObject->AddComponent<Light> ();
            }
            else if (EditorGUI::SelectableLabel ("Rigidbody", false))
            {
                editingGameObject->AddComponent<Rigidbody> ();
            }
            else if (EditorGUI::SelectableLabel ("Box Collider", false))
            {
                editingGameObject->AddComponent<BoxCollider> ();
            }
            else if (EditorGUI::SelectableLabel ("Sphere Collider", false))
            {
                editingGameObject->AddComponent<SphereCollider> ();
            }

            EditorGUI::EndPopup ();
        }
    }
}
