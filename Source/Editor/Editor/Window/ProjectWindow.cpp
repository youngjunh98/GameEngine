#include "ProjectWindow.h"
#include "Editor/Core/EditorGUI.h"
#include "Editor/Editor/Editor.h"
#include "Editor/Editor/Modal/CreateMaterialModal.h"
#include "Editor/Editor/Modal/EditMaterialModal.h"
#include "Engine/Core/Asset/AssetManager.h"
#include "Engine/Engine/Rendering/Material.h"

namespace GameEngine
{
    ProjectWindow::ProjectWindow () : EditorWindow ("Project"),
        m_selectedAssetPath (), m_hoveredAsset (nullptr)
    {
    }

    ProjectWindow::~ProjectWindow ()
    {
    }

    void ProjectWindow::OnRender ()
    {
        RenderDirectoryTreeRecursive (PATH ("Assets"), PATH (""));

		if (EditorGUI::BeginPopupContext ("EditAssetPopup"))
        {
            if (EditorGUI::SelectableLabel ("Create Material", false))
            {
				Editor::GetInstance ().OpenModalWindow<CreateMaterialModal> ();
            }

            if (EditorGUI::SelectableLabel ("Edit", false, m_hoveredAsset == nullptr))
            {
                PathString extension = FileSystem::GetFileExtension (AssetManager::GetInstance ().GetAssetName (m_hoveredAsset));

                if (extension == PATH ("material"))
                {
                    Material* material = dynamic_cast<Material*> (m_hoveredAsset);
                        
                    if (material != nullptr)
                    {
                        EditMaterialModal* materialModal = Editor::GetInstance ().OpenModalWindow<EditMaterialModal> ();
                            
                        if (materialModal != nullptr)
                        {
                            materialModal->SetMaterial (material);
                        }
                    }
                }
            }

			/*
            if (EditorGUI::SelectableLabel ("Delete Asset", false))
            {
            }
			*/

            EditorGUI::EndPopupContext ();
        }
        else
        {
            m_hoveredAsset = nullptr;
        }
    }

    void ProjectWindow::RenderDirectoryTreeRecursive (PathString directoryName, PathString pathToDirectory)
    {
        PathString path = FileSystem::CombinePath (pathToDirectory, directoryName);

        for (PathString directory : FileSystem::GetDirectoryList (path))
        {
            std::string asciiDirectory (directory.begin (), directory.end ());
            bool bSelected = true;

            if (EditorGUI::TreeBeginNode (asciiDirectory))
            {
                RenderDirectoryTreeRecursive (directory, path);
                EditorGUI::TreePopNode ();
            }
        }

        for (PathString file : FileSystem::GetFileList (path))
        {
            std::string asciiFile (file.begin (), file.end ());

            PathString filePath = path + PATH ("/") + file;
            bool bSelected = filePath == m_selectedAssetPath;
            
            if (EditorGUI::SelectableLabel (asciiFile.c_str (), bSelected))
            {
                m_selectedAssetPath = filePath;
            }

            if (EditorGUI::IsLastItemHovered () && m_hoveredAsset == nullptr)
            {
                m_hoveredAsset = AssetManager::GetInstance ().FindAsset<Object> (filePath);;
            }
        }
    }
}