#include "CreateMaterialModal.h"
#include "Editor/Editor/Editor.h"
#include "Editor/Core/EditorGUI.h"
#include "Engine/Core/File/FileSystem.h"
#include "Engine/Core/File/File.h"
#include "Engine/Core/Asset/AssetManager.h"
#include "Engine/Engine/Rendering/Material.h"

namespace GameEngine
{
    CreateMaterialModal::CreateMaterialModal () : EditorModalWindow ("Create Material"),
        m_assetPath ("")
    {
    }

    CreateMaterialModal::~CreateMaterialModal ()
    {
    }

    void CreateMaterialModal::OnRender ()
    {
        EditorGUI::Label ("Material Path");

        EditorGUI::Label ("Assets/");
        EditorGUI::SameLine ();
        m_assetPath = EditorGUI::InputText ("##path", m_assetPath);

        if (EditorGUI::Button ("Create"))
        {
            PathString path = PATH ("Assets/") + PathString (m_assetPath.begin (), m_assetPath.end ());
            File file (path, EFileAccessMode::Write);

            if (file.IsOpen ())
            {
                auto material = std::make_shared<Material> ();
                material->SetShader (AssetManager::GetInstance ().FindAsset<Shader> (PATH ("Assets/Shader/StandardShader.hlsl")));

                std::string json = Json::JsonSerializer::Serialize (*material).dump ();
                int64 jsonSize = json.size ();
                file.Write (json.data (), jsonSize);

                AssetInfo asset;
                asset.m_type = EAssetType::Material;
                asset.m_asset = material;

                AssetManager::GetInstance ().AddAsset (asset, path);
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
