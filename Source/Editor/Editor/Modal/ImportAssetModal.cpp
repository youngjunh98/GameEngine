#include "ImportAssetModal.h"
#include "Engine/Platform/Platform.h"
#include "Engine/Engine/Asset/AssetImporter.h"
#include "Editor/Core/EditorGUI.h"

namespace GameEngine
{
    ImportAssetModal::ImportAssetModal () : EditorModalWindow ("Import Asset"),
		m_path (FileSystem::CombinePath (FileSystem::RemoveFileName (Platform::GetGenericApplication ().GetExecutablePath ()), PATH ("Assets")))
    {
    }

    ImportAssetModal::~ImportAssetModal ()
    {
    }

    void ImportAssetModal::OnRender ()
    {
		RenderDirectory ();

		if (EditorGUI::Button ("Import"))
		{
			if (AssetImporter::ImportAsset (m_path, false))
			{
				Close ();
			}
		}

		EditorGUI::SameLine ();

		if (EditorGUI::Button ("Cancel"))
		{
			Close ();
		}
    }

	void ImportAssetModal::RenderDirectory ()
	{
		if (EditorGUI::SelectableLabel ("..", false))
		{
			m_path = FileSystem::GetParentDirectory (m_path);
			return;
		}

		PathString directoryPath = FileSystem::RemoveFileName (m_path);

		for (PathString directory : FileSystem::GetDirectoryNames (directoryPath))
		{
			std::string asciiDirectory (directory.begin (), directory.end ());

			if (EditorGUI::SelectableLabel (asciiDirectory.c_str (), false))
			{
				m_path = FileSystem::CombinePath (m_path, directory);
				return;
			}
		}

		for (PathString file : FileSystem::GetFileNames (directoryPath))
		{
			std::string asciiFile (file.begin (), file.end ());

			PathString filePath = FileSystem::CombinePath (directoryPath, file);
			bool bSelected = filePath == m_path;

			if (EditorGUI::SelectableLabel (asciiFile.c_str (), bSelected))
			{
				m_path = filePath;
			}
		}
	}
}
