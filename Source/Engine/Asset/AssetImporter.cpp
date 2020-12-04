#include <fstream>
#include <vector>
#include <cwctype>
#include <stack>


#include "AssetImporter.h"
#include "AssetManager.h"
#include "3D/OBJImporter.h"
#include "3D/FbxImporter.h"
#include "Image/StbImageImporter.h"
#include "Audio/WavImporter.h"
#include "Core/Json/JsonSerializer.h"
#include "Core/File/File.h"
#include "Core/File/FileSystem.h"
#include "Engine/Rendering/Material.h"
#include "Engine/Scene/SceneManager.h"


namespace GameEngine
{
	void AssetImporter::ImportAllAssets ()
	{
		FBXImporter::Start ();

		std::wstring currentDirectory;
		std::stack<std::wstring> directoryStack;

		directoryStack.push (PATH ("Assets"));

		while (directoryStack.size () > 0)
		{
			auto topDirectory = directoryStack.top ();
			bool topDirectoryIterated = currentDirectory.rfind (topDirectory + PATH ("/")) != std::string::npos;

			if (topDirectoryIterated == false)
			{
				currentDirectory += topDirectory + PATH ("/");

				for (auto& file : FileSystem::GetFileList (currentDirectory))
				{
					auto path = currentDirectory + file;

					if (g_assetManager.FindAsset<Object> (path) == nullptr)
					{
						ImportAsset (path);
					}
				}

				for (auto& directory : FileSystem::GetDirectoryList (currentDirectory))
				{
					directoryStack.push (directory);
				}
			}

			if (topDirectory == directoryStack.top ())
			{
				auto topDirectoryFind = currentDirectory.rfind (topDirectory + PATH ("/"));
				currentDirectory.erase (currentDirectory.begin () + topDirectoryFind, currentDirectory.end ());

				directoryStack.pop ();
			}
		}

		FBXImporter::Shutdown ();
	}

	bool AssetImporter::ImportAsset (const std::wstring& path)
	{
		auto extension = FileSystem::GetFileExtension (path);
		std::shared_ptr<Object> imported = nullptr;

		if (extension == PATH ("obj") || extension == PATH ("fbx"))
		{
			imported = ImportMesh (path);
		}
		else if (extension == PATH ("jpg") || extension == PATH ("jpeg") || extension == PATH ("png") || extension == PATH ("tga"))
		{
			imported = ImportTexture2D (path, true);
		}
		else if (extension == PATH ("wav"))
		{
			imported = ImportAudioClip (path);
		}
		else if (extension == PATH ("scene"))
		{
			File file (path, EFileAccessMode::Read);
			int64 fileSize = file.GetSize ();

			std::string jsonString (fileSize + 1, L'\0');
			auto* first = const_cast<char*> (jsonString.data ());

			file.ReadAll (first);
			Json::Json sceneData = Json::Json::parse (jsonString);

			g_sceneManager.RegisterScene (path, sceneData);

			return true;
		}
		else if (extension == PATH ("material"))
		{
			File file (path, EFileAccessMode::Read);
			int64 fileSize = file.GetSize ();

			std::string jsonString (fileSize + 1, '\0');
			auto* first = const_cast<char*> (jsonString.data ());

			file.ReadAll (first);
			Json::Json materialData = Json::Json::parse (jsonString, nullptr, false);

			auto material = std::make_shared<Material> ();
			material->OnDeserialize (materialData);

			imported = material;
		}

		if (imported != nullptr)
		{
			g_assetManager.AddAsset (imported, path);
		}

		return imported != nullptr;
	}

	std::shared_ptr<Mesh> AssetImporter::ImportMesh (const std::wstring& path)
	{
		std::unique_ptr<int8[]> fileData;
		int64 fileSize;

		if (GetFileDataAndSize (path, fileData, fileSize) == false)
		{
			return nullptr;
		}

		auto mesh = std::make_shared<Mesh> ();
		auto extension = FileSystem::GetFileExtension (path);

		if (extension == PATH ("obj"))
		{
			if (OBJImporter::Import (*mesh, fileData.get (), fileSize) == false)
			{
				return nullptr;
			}
		}
		else if (extension == PATH ("fbx"))
		{
			if (FBXImporter::Import (*mesh, fileData.get (), fileSize) == false)
			{
				return nullptr;
			}
		}
		else
		{
			return nullptr;
		}

		return mesh;
	}

	std::shared_ptr<Texture2D> AssetImporter::ImportTexture2D (const std::wstring& path, bool bGenerateMipMaps)
	{
		std::unique_ptr<int8[]> fileData;
		int64 fileSize;

		if (GetFileDataAndSize (path, fileData, fileSize) == false)
		{
			return nullptr;
		}

		auto image = std::make_shared<StbImage>();

		if (StbImageImporter::Import (*image, fileData.get (), fileSize, true, false) == false)
		{
			return nullptr;
		}

		TextureResourceData data;
		data.m_data = image->m_data;
		data.m_dataPitch = image->GetRowPitch ();

		std::vector<TextureResourceData> textureData = { data };
		std::vector<std::shared_ptr<StbImage>> mipMaps;

		if (bGenerateMipMaps && image->IsPowerOfTwo () && image->IsSquare ())
		{
			for (int32 mipMapSize = image->m_width / 2; mipMapSize > 0; mipMapSize /= 2)
			{
				auto mipMap = std::make_shared<StbImage> ();

				if (StbImageImporter::Resize (*mipMap, *image, mipMapSize, mipMapSize) == false)
				{
					return nullptr;
				}

				TextureResourceData mipMapData;
				mipMapData.m_data = mipMap->m_data;
				mipMapData.m_dataPitch = mipMap->GetRowPitch ();

				textureData.push_back (mipMapData);
				mipMaps.push_back (mipMap);
			}
		}

		auto texture2D = std::make_shared<Texture2D> ();
		texture2D->SetWidth (image->m_width);
		texture2D->SetHeight (image->m_height);
		texture2D->SetFormat (image->GetFormat ());

		if (texture2D->UpdateTextureResource (textureData) == false)
		{
			return nullptr;
		}

		texture2D->SetFilterMode (EFilterMode::Anisotropic);
		texture2D->SetAnisotropicLevel (16);
		texture2D->SetAddressMode (EAddressMode::Wrap);

		if (texture2D->UpdateSamplerResource () == false)
		{
			return nullptr;
		}

		//g_assetManager.AddAsset (texture2D, path);

		return texture2D;
	}

	std::shared_ptr<TextureCube> AssetImporter::ImportTextureCube (const std::wstring cubePath[6])
	{
		// 0 = Right, 1 = Left, 2 = Up, 3 = Down, 4 = Front, 5 = Back
		std::shared_ptr<StbImage> cube[6];
		cube[0] = std::make_shared<StbImage> ();
		cube[1] = std::make_shared<StbImage> ();
		cube[2] = std::make_shared<StbImage> ();
		cube[3] = std::make_shared<StbImage> ();
		cube[4] = std::make_shared<StbImage> ();
		cube[5] = std::make_shared<StbImage> ();

		for (int32 i = 0; i < 6; i++)
		{
			std::unique_ptr<int8[]> fileData;
			int64 fileSize;

			if (GetFileDataAndSize (cubePath[i], fileData, fileSize) == false)
			{
				return nullptr;
			}

			if (StbImageImporter::Import (*cube[i], fileData.get (), fileSize, true, false) == false)
			{
				return nullptr;
			}
		}

		if (cube[0]->m_width != cube[1]->m_width || cube[0]->m_width != cube[2]->m_width ||
			cube[0]->m_width != cube[3]->m_width || cube[0]->m_width != cube[4]->m_width ||
			cube[0]->m_width != cube[5]->m_width)
		{
			return nullptr;
		}


		if (cube[0]->m_height != cube[1]->m_height || cube[0]->m_height != cube[2]->m_height ||
			cube[0]->m_height != cube[3]->m_height || cube[0]->m_height != cube[4]->m_height ||
			cube[0]->m_height != cube[5]->m_height)
		{
			return nullptr;
		}

		std::vector<TextureResourceData> textureData;
		std::vector<std::shared_ptr<StbImage>> mipMaps;

		if (cube[0]->IsPowerOfTwo () && cube[0]->IsSquare () &&
			cube[1]->IsPowerOfTwo () && cube[1]->IsSquare () &&
			cube[2]->IsPowerOfTwo () && cube[2]->IsSquare () &&
			cube[3]->IsPowerOfTwo () && cube[3]->IsSquare () &&
			cube[4]->IsPowerOfTwo () && cube[4]->IsSquare () &&
			cube[5]->IsPowerOfTwo () && cube[5]->IsSquare ())
		{
			for (int32 i = 0; i < 6; i++)
			{
				TextureResourceData data;
				data.m_data = cube[i]->m_data;
				data.m_dataPitch = cube[i]->GetRowPitch ();

				textureData.push_back (data);

				for (int32 mipMapSize = cube[i]->m_width / 2; mipMapSize > 0; mipMapSize /= 2)
				{
					auto mipMap = std::make_shared<StbImage> ();

					if (StbImageImporter::Resize (*mipMap, *cube[i], mipMapSize, mipMapSize) == false)
					{
						return nullptr;
					}

					TextureResourceData mipMapData;
					mipMapData.m_data = mipMap->m_data;
					mipMapData.m_dataPitch = mipMap->GetRowPitch ();

					textureData.push_back (mipMapData);
					mipMaps.push_back (mipMap);
				}
			}
		}

		auto textureCube = std::make_shared<TextureCube> ();
		textureCube->SetWidth (cube[0]->m_width);
		textureCube->SetHeight (cube[0]->m_height);
		textureCube->SetFormat (cube[0]->GetFormat ());

		if (textureCube->UpdateTextureResource (textureData) == false)
		{
			return nullptr;
		}

		textureCube->SetFilterMode (EFilterMode::Anisotropic);
		textureCube->SetAnisotropicLevel (16);
		textureCube->SetAddressMode (EAddressMode::Wrap);

		if (textureCube->UpdateSamplerResource () == false)
		{
			return nullptr;
		}

		g_assetManager.AddAsset (textureCube, cubePath[0]);

		return textureCube;
	}

	std::shared_ptr<AudioClip> AssetImporter::ImportAudioClip (const std::wstring& path)
	{
		std::unique_ptr<int8[]> fileData;
		int64 fileSize;

		if (GetFileDataAndSize (path, fileData, fileSize) == false)
		{
			return nullptr;
		}

		auto audioClip = std::make_shared<AudioClip> ();

		if (WavImporter::Import (*audioClip, fileData.get (), fileSize) == false)
		{
			return nullptr;
		}

		//g_assetManager.AddAsset (audioClip, path);

		return audioClip;
	}

	bool AssetImporter::GetFileDataAndSize (const std::wstring& path, std::unique_ptr<int8[]>& data, int64& size)
	{
		File file (path, EFileAccessMode::Read);

		if (file.IsOpen () == false)
		{
			return false;
		}

		int64 fileSize = file.GetSize ();

		if (fileSize <= 0)
		{
			return false;
		}

		data = std::make_unique<int8[]> (fileSize);
		size = file.ReadAll (data.get ());

		return true;
	}
}