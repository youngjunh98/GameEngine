#include "AssetImporter.h"
#include "AssetData.h"
#include "Engine/Core/Asset/AssetManager.h"
#include "Engine/Core/Asset/3D/OBJImporter.h"
#include "Engine/Core/Asset/3D/FbxImporter.h"
#include "Engine/Core/Asset/Image/StbImageImporter.h"
#include "Engine/Core/Asset/Audio/WavImporter.h"
#include "Engine/Core/Json/JsonSerializer.h"
#include "Engine/Core/File/File.h"
#include "Engine/Core/File/FileSystem.h"
#include "Engine/Engine/Rendering/Material.h"
#include "Engine/Engine/Scene/SceneManager.h"

namespace GameEngine
{
	void AssetImporter::ImportAllAssets ()
	{
		ImportAllAssetsRecursive (PATH ("Assets"));
	}

	bool AssetImporter::ImportAsset (const PathString& path)
	{
		AssetManager& assetManager = AssetManager::GetInstance ();

		bool bSucceed = false;
		bool bNoAssetFound = assetManager.FindAsset<Object> (path) == nullptr;

		if (bNoAssetFound)
		{
			AssetInfo asset;
			auto extension = FileSystem::GetFileExtension (path);

			if (IsSupported3D (extension))
			{
				asset.m_type = EAssetType::Mesh;
				asset.m_asset = Import3D (path);
			}
			else if (IsSupported2D (extension))
			{
				asset.m_type = EAssetType::Texture;
				asset.m_asset = Import2D (path, true);
			}
			else if (IsSupportedAudio (extension))
			{
				asset.m_type = EAssetType::Audio;
				asset.m_asset = ImportAudio (path);
			}
			else if (extension == PATH ("hlsl"))
			{
				asset.m_type = EAssetType::Shader;
				asset.m_asset = LoadShader (path);
			}
			else if (extension == PATH ("scene"))
			{
				File file (path, EFileAccessMode::Read);
				int64 fileSize = file.GetSize ();

				std::string jsonString (fileSize + 1, L'\0');
				auto* first = const_cast<char*> (jsonString.data ());

				file.ReadAll (first);
				Json::Json sceneData = Json::Json::parse (jsonString);

				SceneManager::GetInstance ().RegisterScene (path, sceneData);

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

				asset.m_type = EAssetType::Material;
				asset.m_asset = material;
			}

			if (asset.m_asset != nullptr)
			{
				bSucceed = true;
				assetManager.AddAsset (asset, path);
			}
		}

		return bSucceed;
	}

	std::shared_ptr<TextureCube> AssetImporter::ImportTextureCube (const PathString cubePath[6])
	{
		// 0 = Right, 1 = Left, 2 = Up, 3 = Down, 4 = Front, 5 = Back
		const uint32 arraySize = 6;
		StbImage cube[arraySize];

		for (uint32 i = 0; i < arraySize; i++)
		{
			std::unique_ptr<int8[]> fileData;
			int64 fileSize;

			if (GetFileDataAndSize (cubePath[i], fileData, fileSize) == false)
			{
				return nullptr;
			}

			if (StbImageImporter::Import (cube[i], fileData.get (), fileSize, true, false) == false)
			{
				return nullptr;
			}
		}

		if (cube[0].IsSquare () == false || cube[0].IsPowerOfTwo () == false)
		{
			return nullptr;
		}

		for (uint32 i = 1; i < arraySize; i++)
		{
			if (cube[i].m_width != cube[0].m_width || cube[i].m_height != cube[0].m_height)
			{
				return nullptr;
			}
		}

		std::vector<TextureData> textureData;

		for (uint32 i = 0; i < arraySize; i++)
		{
			StbImage& image = cube[i];

			uint8* imageData = static_cast<uint8*> (image.m_data);
			TextureData data;
			data.m_dataBytes = image.GetDataSizeInBytes ();
			data.m_dataRowBytes = image.GetRowSizeInBytes ();
			data.m_data = std::vector<uint8> (imageData, imageData + data.m_dataBytes);

			textureData.push_back (data);

			for (int32 mipMapSize = image.m_width / 2; mipMapSize > 0; mipMapSize /= 2)
			{
				StbImage mipMap;

				if (StbImageImporter::Resize (mipMap, image, mipMapSize, mipMapSize) == false)
				{
					return nullptr;
				}

				uint8* mipMapImageData = static_cast<uint8*> (mipMap.m_data);
				TextureData mipMapData;
				mipMapData.m_dataBytes = mipMap.GetDataSizeInBytes ();
				mipMapData.m_dataRowBytes = mipMap.GetRowSizeInBytes ();
				mipMapData.m_data = std::vector<uint8> (mipMapImageData, mipMapImageData + mipMapData.m_dataBytes);

				textureData.push_back (mipMapData);
			}
		}

		auto textureCube = std::make_shared<TextureCube> ();
		textureCube->SetWidth (cube[0].m_width);
		textureCube->SetHeight (cube[0].m_height);
		textureCube->SetFormat (cube[0].GetFormat ());
		textureCube->SetTextureData (textureData);

		if (textureCube->UpdateTextureResource () == false)
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

		AssetInfo asset;
		asset.m_type = EAssetType::Texture;
		asset.m_asset = textureCube;
		AssetManager::GetInstance ().AddAsset (asset, cubePath[0]);

		return textureCube;
	}

	void AssetImporter::ImportAllAssetsRecursive (const PathString& path)
	{
		for (const PathString& fileName : FileSystem::GetFileList (path))
		{
			PathString filePath = path + PATH ("/") + fileName;
			ImportAsset (filePath);
		}

		for (const PathString& directoryName : FileSystem::GetDirectoryList (path))
		{
			PathString directoryPath = path + PATH ("/") + directoryName;
			ImportAllAssetsRecursive (directoryPath);
		}
	}

	bool AssetImporter::IsSupported3D (const PathString& extension)
	{
		return extension == PATH ("obj") || extension == PATH ("fbx");
	}

	bool AssetImporter::IsSupported2D (const PathString& extension)
	{
		return extension == PATH ("jpg") || extension == PATH ("jpeg") ||
			extension == PATH ("png") || extension == PATH ("tga") || extension == PATH ("hdr");
	}

	bool AssetImporter::IsSupportedAudio (const PathString& extension)
	{
		return extension == PATH ("wav");
	}

	std::shared_ptr<Shader> AssetImporter::LoadShader (const PathString& path)
	{
		auto shader = std::make_shared<Shader> ();

		if (shader != nullptr)
		{
			if (shader->Initialize (path) == false)
			{
				shader = nullptr;
			}
		}

		return shader;
	}

	std::shared_ptr<Mesh> AssetImporter::Import3D (const PathString& path)
	{
		std::unique_ptr<int8[]> fileData;
		int64 fileSize;

		if (GetFileDataAndSize (path, fileData, fileSize) == false)
		{
			return nullptr;
		}

		MeshData meshData;
		PathString extension = FileSystem::GetFileExtension (path);

		if (extension == PATH ("obj"))
		{
			meshData = OBJImporter::Import (fileData.get (), fileSize);
		}
		else if (extension == PATH ("fbx"))
		{
			FBXImporter::Start ();
			meshData = FBXImporter::Import (fileData.get (), fileSize);
			FBXImporter::Shutdown ();
		}

		auto mesh = std::make_shared<Mesh> ();

		if (mesh != nullptr)
		{
			mesh->SetSubMeshCount (meshData.m_subMeshData.size ());

			for (uint32 subMeshIndex = 0; subMeshIndex < meshData.m_subMeshData.size (); subMeshIndex++)
			{
				mesh->SetVertices (subMeshIndex, meshData.m_subMeshData.at (subMeshIndex).m_vertices);

				if (mesh->UpdateVertexBufferResource (subMeshIndex) == false)
				{
					mesh = nullptr;
					break;
				}
			}
		}

		return mesh;
	}

	std::shared_ptr<Texture2D> AssetImporter::Import2D (const PathString& path, bool bGenerateMipMaps)
	{
		std::unique_ptr<int8[]> fileData;
		int64 fileSize;

		if (GetFileDataAndSize (path, fileData, fileSize) == false)
		{
			return nullptr;
		}

		StbImage image;

		if (StbImageImporter::Import (image, fileData.get (), fileSize, true, false) == false)
		{
			return nullptr;
		}

		uint8* imageData = static_cast<uint8*> (image.m_data);
		TextureData data;
		data.m_dataBytes = image.GetDataSizeInBytes ();
		data.m_dataRowBytes = image.GetRowSizeInBytes ();
		data.m_data = std::vector<uint8> (imageData, imageData + data.m_dataBytes);

		std::vector<TextureData> textureData = { data };

		if (bGenerateMipMaps && image.IsPowerOfTwo () && image.IsSquare ())
		{
			for (int32 mipMapSize = image.m_width / 2; mipMapSize > 0; mipMapSize /= 2)
			{
				StbImage mipMap;

				if (StbImageImporter::Resize (mipMap, image, mipMapSize, mipMapSize) == false)
				{
					return nullptr;
				}

				uint8* mipMapImageData = static_cast<uint8*> (mipMap.m_data);
				TextureData mipMapData;
				mipMapData.m_dataBytes = mipMap.GetDataSizeInBytes ();
				mipMapData.m_dataRowBytes = mipMap.GetRowSizeInBytes ();
				mipMapData.m_data = std::vector<uint8> (mipMapImageData, mipMapImageData + mipMapData.m_dataBytes);

				textureData.push_back (mipMapData);
			}
		}

		auto texture2D = std::make_shared<Texture2D> ();
		texture2D->SetWidth (image.m_width);
		texture2D->SetHeight (image.m_height);
		texture2D->SetFormat (image.GetFormat ());
		texture2D->SetTextureData (textureData);

		if (texture2D->UpdateTextureResource () == false)
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

		return texture2D;
	}

	std::shared_ptr<AudioClip> AssetImporter::ImportAudio (const PathString& path)
	{
		std::unique_ptr<int8[]> fileData;
		int64 fileSize;

		if (GetFileDataAndSize (path, fileData, fileSize) == false)
		{
			return nullptr;
		}

		AudioData audioData = WavImporter::Import (fileData.get (), fileSize);
		auto audioClip = std::make_shared<AudioClip> ();

		if (audioClip != nullptr)
		{
			audioClip->SetSampleData (audioData.m_sampleData, audioData.m_channelCount, audioData.m_sampleRate);
		}

		return audioClip;
	}

	bool AssetImporter::GetFileDataAndSize (const PathString& path, std::unique_ptr<int8[]>& data, int64& size)
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