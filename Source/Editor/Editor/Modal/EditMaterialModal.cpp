#include "EditMaterialModal.h"
#include "Editor/Core/EditorGUI.h"
#include "Engine/Core/Asset/AssetManager.h"

namespace GameEngine
{
    EditMaterialModal::EditMaterialModal () : EditorModalWindow ("Edit Material"),
        m_material (nullptr)
    {
    }

    EditMaterialModal::~EditMaterialModal ()
    {
    }

    void EditMaterialModal::OnRender ()
    {
        if (m_material != nullptr)
        {
            m_material->OnRenderEditor ();
        }

        if (EditorGUI::Button ("Save"))
        {
            AssetManager::GetInstance ().SaveAsset (AssetManager::GetInstance ().GetAssetPath (m_material));
            Close ();
        }

        EditorGUI::SameLine ();

        if (EditorGUI::Button ("Cancel"))
        {
            Close ();
        }
    }

    Material* EditMaterialModal::GetMaterial () const
    {
        return m_material;
    }

    void EditMaterialModal::SetMaterial (Material* material)
    {
        m_material = material;
    }
}
