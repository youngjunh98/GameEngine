#include "CreateMaterialModal.h"
#include "Editor/Editor/Editor.h"
#include "Editor/Core/EditorGUI.h"
#include "Engine/Core/File/FileSystem.h"
#include "Engine/Core/File/File.h"
#include "Engine/Core/Asset/AssetManager.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/Engine/Rendering/GlobalRenderer.h"
#include "Engine/Engine/Rendering/Material.h"

namespace GameEngine
{
    CreateMaterialModal::CreateMaterialModal () : EditorModalWindow ("Create Material"),
        m_name ("")
    {
    }

    CreateMaterialModal::~CreateMaterialModal ()
    {
    }

    void CreateMaterialModal::OnRender ()
    {
        EditorGUI::Label ("Material Path");

        EditorGUI::Label ("Name");
        EditorGUI::SameLine ();
        m_name = EditorGUI::InputText ("##name", m_name);

        if (EditorGUI::Button ("Create"))
        {
            PathString name = FileSystem::SetFileExtension (PathString (m_name.begin (), m_name.end ()), PATH("asset"));
            PathString path = FileSystem::CombinePath (PATH ("Assets"), name);
            File file (path, EFileAccessMode::Write);

            if (file.IsOpen ())
            {
                auto material = std::make_shared<Material> ();
                material->SetShader (GlobalRenderer::GetStandardShader (). get());

                AssetManager::AddAsset (material, path);
                AssetManager::SaveAsset (path);
            }

            Close ();
        }

        EditorGUI::SameLine ();

        if (EditorGUI::Button ("Cancel"))
        {
            Close ();
        }
    }
}
