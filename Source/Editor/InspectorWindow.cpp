#include "InspectorWindow.h"
#include "Editor.h"
#include "DearImGui.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Transform.h"
#include "Engine/Component/Camera.h"
#include "Engine/Component/Physics/Rigidbody.h"
#include "Engine/Component/Physics/BoxCollider.h"
#include "Engine/Component/Physics/SphereCollider.h"
#include "Engine/Component/Rendering/MeshRenderer.h"
#include "Engine/Component/Rendering/Light.h"

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

        bool bActive = editingGameObject->IsActive ();

        if (ImGui::Checkbox ("##active-checkbox", &bActive))
        {
            editingGameObject->SetActive (bActive);
        }

        ImGui::SameLine ();

        std::string name = editingGameObject->GetName ();
        std::string tag = editingGameObject->GetTag ();

        ImGui::Text ("Name");
        ImGui::SameLine ();

        if (ImGui::InputText ("##name-label", &name, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            editingGameObject->SetName (name);
        }

        ImGui::Text ("Tag");
        ImGui::SameLine ();

        if (ImGui::InputText ("##tag-label", &tag, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            editingGameObject->SetTag (tag);
        }

        int32 id = 0;

        for (auto* component : editingGameObject->GetComponents<Component> ())
        {
            if (component->IsAlive () == false)
            {
                continue;
            }

            ImGui::Separator ();
            ImGui::PushID (id);

            component->OnRenderEditor (editor);

            ImGui::PopID ();
            id += 1;
        }

        ImGui::Separator ();
        ImGui::Spacing ();

        if (ImGui::Button ("Add Component"))
        {
            ImGui::OpenPopup ("##add-component-popup");
        }

        if (ImGui::BeginPopup ("##add-component-popup"))
        {
            if (ImGui::Selectable ("Camera"))
            {
                editingGameObject->AddComponent<Camera> ();
            }
            else if (ImGui::Selectable ("Mesh Renderer"))
            {
                editingGameObject->AddComponent<MeshRenderer> ();
            }
            else if (ImGui::Selectable ("Light"))
            {
                editingGameObject->AddComponent<Light> ();
            }
            else if (ImGui::Selectable ("Rigidbody"))
            {
                editingGameObject->AddComponent<Rigidbody> ();
            }
            else if (ImGui::Selectable ("Box Collider"))
            {
                editingGameObject->AddComponent<BoxCollider> ();
            }
            else if (ImGui::Selectable ("Sphere Collider"))
            {
                editingGameObject->AddComponent<SphereCollider> ();
            }

            ImGui::EndPopup ();
        }
    }
}
