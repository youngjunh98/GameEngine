#include "CreateMaterialModal.h"
#include "Editor/Editor.h"
#include "Editor/EditorGUI.h"
#include "Core/File/FileSystem.h"
#include "Core/File/File.h"
#include "Engine/Asset/AssetManager.h"
#include "Engine/Rendering/Material.h"

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
                material->SetShader (g_assetManager.LoadShader (PATH ("Assets/Shader/StandardShader.hlsl")));

                std::string json = Json::JsonSerializer::Serialize (*material).dump ();
                int64 jsonSize = json.size ();
                file.Write (json.data (), jsonSize);

                g_assetManager.AddAsset (material, path);
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
