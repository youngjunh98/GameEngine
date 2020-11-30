#include "ProjectWindow.h"
#include "Editor.h"
#include "DearImGui.h"

namespace GameEngine
{
    ProjectWindow::ProjectWindow () : EditorWindow ("Project"), m_selectedAssetPath ()
    {
    }

    ProjectWindow::~ProjectWindow ()
    {
    }

    void ProjectWindow::OnRender ()
    {
        RenderDirectoryTreeRecursive (PATH ("Assets"), PATH (""));

        /*
		if (ImGui::BeginPopupContextWindow ("##create-asset"))
		{
			if (ImGui::Selectable ("Create Material"))
			{
				m_bCreatingAsset = true;
			}

			ImGui::EndPopup ();
		}

		if (m_bCreatingAsset)
		{
			m_bCreatingAsset = false;
			ImGui::OpenPopup ("##create-asset-popup");
		}

		if (ImGui::BeginPopupModal ("##create-asset-popup"))
		{
			ImGui::Text ("Path");

			ImGui::Text ("Assets/");
			ImGui::SameLine ();

			ImGui::InputText ("##create-asset-path", &m_createAssetPath, ImGuiInputTextFlags_None);
			std::string path = "Assets/" + m_createAssetPath;
			std::wstring unicodePath (path.begin (), path.end ());

			if (ImGui::Button ("Create"))
			{
				File file (unicodePath, EFileAccessMode::Write);

				if (file.IsOpen ())
				{
					auto material = std::make_shared<Material> ();
					material->SetShader (g_assetManager.LoadShader (L"Assets/Shader/StandardShader.hlsl"));

					std::string jsonString = Json::JsonSerializer::Serialize (*material).dump ();
					int64 jsonStringSize = jsonString.size ();

					file.Write (jsonString.data (), jsonStringSize);

					g_assetManager.AddAsset (material, unicodePath);

					m_selectedAssetPath = unicodePath;
					m_bShowAssetEditWindow = true;
				}

				ImGui::CloseCurrentPopup ();
			}

			ImGui::SameLine ();

			if (ImGui::Button ("Cancel"))
			{
				ImGui::CloseCurrentPopup ();
			}

			ImGui::EndPopup ();
		}
        */
    }

    void ProjectWindow::RenderDirectoryTreeRecursive (PathString directoryName, PathString pathToDirectory)
    {
        PathString path = FileSystem::CombinePath (pathToDirectory, directoryName);

        for (PathString directory : FileSystem::GetDirectoryList (path))
        {
            std::string asciiDirectory (directory.begin (), directory.end ());
            bool bSelected = true;

            if (ImGui::TreeNode (asciiDirectory.c_str ()))
            {
                RenderDirectoryTreeRecursive (directory, path);
                ImGui::TreePop ();
            }
        }

        for (PathString file : FileSystem::GetFileList (path))
        {
            std::string asciiFile (file.begin (), file.end ());

            PathString filePath = FileSystem::CombinePath (path, file);;
            bool bSelected = filePath == m_selectedAssetPath;
            
            if (ImGui::Selectable (asciiFile.c_str (), bSelected))
            {
                m_selectedAssetPath = filePath;
            }
        }
    }
}