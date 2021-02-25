#include "ImportAssetModal.h"
#include "Engine/Platform/Platform.h"
#include "Engine/Asset/AssetImporter.h"
#include "Engine/Asset/AssetManager.h"
#include "Engine/Engine/Rendering/Mesh.h"
#include "Engine/Engine/Rendering/Texture2D.h"
#include "Engine/Engine/Audio/AudioClip.h"
#include "Editor/Core/EditorGUI.h"

namespace GameEngine
{
	ImportAssetModal::ImportAssetModal () : EditorModalWindow ("Import Asset"),
		m_path (FileSystem::RemoveFileName (Platform::GetGenericApplication ().GetExecutablePath ()))
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
			std::shared_ptr<Object> asset;
			PathString extension = FileSystem::GetFileExtension (m_path);

			if (AssetImporter::IsSupportedMesh (extension))
			{
				auto mesh = std::make_shared<Mesh> ();

				if (ImportMeshAsset (m_path, *mesh))
				{
					asset = mesh;
				}
			}
			else if (AssetImporter::IsSupportedTexture (extension))
			{
				auto texture2D = std::make_shared<Texture2D> ();

				if (ImportTexture2DAsset (m_path, *texture2D))
				{
					asset = texture2D;
				}
			}
			else if (AssetImporter::IsSupportedAudio (extension))
			{
				auto audioClip = std::make_shared<AudioClip> ();

				if (ImportAudioAsset (m_path, *audioClip))
				{
					asset = audioClip;
				}
			}

			if (asset != nullptr)
			{
				RegisterAsset (asset, m_path);
			}

			Close ();
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

	void ImportAssetModal::RegisterAsset (const std::shared_ptr<Object>& asset, const PathString& path)
	{
		PathString assetPath = FileSystem::CombinePath (PATH ("Assets"), FileSystem::GetFileName (path));
		assetPath = FileSystem::SetFileExtension (assetPath, PATH ("asset"));

		AssetManager::AddAsset (asset, assetPath);
		AssetManager::SaveAsset (assetPath);
	}

	bool ImportAssetModal::ImportMeshAsset (const PathString& path, Mesh& mesh)
	{
		MeshData data = AssetImporter::ImportMesh (path);

		if (data.m_subMeshData.size () <= 0)
		{
			return false;
		}

		mesh.SetSubMeshCount (data.m_subMeshData.size ());

		for (uint32 subMeshIndex = 0; subMeshIndex < data.m_subMeshData.size (); subMeshIndex++)
		{
			mesh.SetVertices (subMeshIndex, data.m_subMeshData.at (subMeshIndex).m_vertices);

			if (mesh.UpdateVertexBufferResource (subMeshIndex) == false)
			{
				return false;
			}
		}

		return true;
	}

	bool ImportAssetModal::ImportTexture2DAsset (const PathString& path, Texture2D& texture)
	{
		TextureData data = AssetImporter::ImportTexture (path, true);

		if (data.m_array.size () <= 0 || data.m_format == ERenderingResourceFormat::None)
		{
			return false;
		}

		texture.SetTextureData (data);

		if (texture.UpdateTextureResource () == false)
		{
			return false;
		}

		texture.SetFilterMode (EFilterMode::Linear);
		texture.SetAnisotropicLevel (1);
		texture.SetAddressMode (EAddressMode::Wrap);

		if (texture.UpdateSamplerResource () == false)
		{
			return false;
		}

		return true;
	}

	bool ImportAssetModal::ImportAudioAsset (const PathString& path, AudioClip& audioClip)
	{
		AudioData data = AssetImporter::ImportAudio (path);

		if (data.m_sampleData.size () <= 0 || data.m_channelCount <= 0 || data.m_sampleRate <= 0.0f)
		{
			return false;
		}

		audioClip.SetSampleData (data.m_sampleData, data.m_channelCount, data.m_sampleRate);
		return true;
	}
}
