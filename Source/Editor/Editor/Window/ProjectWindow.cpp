#include "ProjectWindow.h"
#include "Editor/Core/EditorGUI.h"
#include "Editor/Editor/Editor.h"
#include "Editor/Editor/Modal/CreateMaterialModal.h"
#include "Editor/Editor/Modal/EditMaterialModal.h"
#include "Engine/Asset/AssetManager.h"
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
		RenderDirectoryRecursive (PATH ("Assets"), PATH (""));

		if (EditorGUI::BeginPopupContext ("ProjectWindowPopup"))
		{
			RenderCreateMenu ();
			RenderEditMenu ();

			EditorGUI::EndPopupContext ();
		}
		else
		{
			m_hoveredAsset = nullptr;
		}
	}

	void ProjectWindow::RenderDirectoryRecursive (PathString directoryName, PathString pathToDirectory)
	{
		PathString path = FileSystem::CombinePath (pathToDirectory, directoryName);

		for (PathString directory : FileSystem::GetDirectoryNames (path))
		{
			std::string asciiDirectory (directory.begin (), directory.end ());
			bool bSelected = true;

			if (EditorGUI::TreeBeginNode (asciiDirectory))
			{
				RenderDirectoryRecursive (directory, path);
				EditorGUI::TreePopNode ();
			}
		}

		for (PathString file : FileSystem::GetFileNames (path))
		{
			std::string asciiFile (file.begin (), file.end ());

			PathString filePath = FileSystem::CombinePath(path, file);
			bool bSelected = filePath == m_selectedAssetPath;

			if (EditorGUI::SelectableLabel (asciiFile.c_str (), bSelected))
			{
				m_selectedAssetPath = filePath;
			}

			if (EditorGUI::IsLastItemHovered () && m_hoveredAsset == nullptr)
			{
				m_hoveredAsset = AssetManager::GetAsset (filePath).get ();
			}
		}
	}

	void ProjectWindow::RenderCreateMenu ()
	{
		if (EditorGUI::SelectableLabel ("Create Material", false))
		{
			Editor::GetInstance ().OpenModalWindow<CreateMaterialModal> ();
		}
	}

	void ProjectWindow::RenderEditMenu ()
	{
		if (EditorGUI::SelectableLabel ("Edit", false, m_hoveredAsset == nullptr))
		{
			if (m_hoveredAsset == nullptr)
			{
				return;
			}

			std::string type = m_hoveredAsset->GetTypeName ();

			if (type == "Material")
			{
				Material* material = dynamic_cast<Material*> (m_hoveredAsset);
				EditMaterialModal* materialModal = Editor::GetInstance ().OpenModalWindow<EditMaterialModal> ();

				if (materialModal != nullptr)
				{
					materialModal->SetMaterial (material);
				}
			}
		}
	}
}