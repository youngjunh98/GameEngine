#include "AssetImporter.h"
#include "AssetManager.h"
#include "3D/OBJImporter.h"
#include "3D/FbxImporter.h"
#include "Image/StbImage.h"
#include "Audio/WavImporter.h"
#include "Engine/Core/File/File.h"

namespace GameEngine
{
	AssetImporter::AssetImporter ()
	{}

	AssetImporter::~AssetImporter ()
	{}

	bool AssetImporter::IsSupportedMesh (const PathString& fileExtension)
	{
		return fileExtension == PATH ("obj") || fileExtension == PATH ("fbx");
	}

	bool AssetImporter::IsSupportedTexture (const PathString& fileExtension)
	{
		return fileExtension == PATH ("jpg") || fileExtension == PATH ("jpeg") ||
			fileExtension == PATH ("png") || fileExtension == PATH ("tga") || fileExtension == PATH ("hdr");
	}

	bool AssetImporter::IsSupportedAudio (const PathString& fileExtension)
	{
		return fileExtension == PATH ("wav");
	}

	MeshData AssetImporter::ImportMesh (const PathString& path)
	{
		MeshData result;
		PathString extension = FileSystem::GetFileExtension (path);

		if (IsSupportedMesh (extension))
		{
			std::unique_ptr<uint8[]> fileData;
			int64 fileSize;

			if (GetFileDataAndSize (path, fileData, fileSize))
			{
				if (extension == PATH ("obj"))
				{
					result = OBJImporter::Import (fileData.get (), fileSize);
				}
				else if (extension == PATH ("fbx"))
				{
					FBXImporter::Start ();
					result = FBXImporter::Import (fileData.get (), fileSize);
					FBXImporter::Shutdown ();
				}
			}
		}

		return result;
	}

	TextureData AssetImporter::ImportTexture (const PathString& path, bool bGenerateMipMaps)
	{
		TextureData result;
		PathString extension = FileSystem::GetFileExtension (path);

		if (IsSupportedTexture (extension))
		{
			std::unique_ptr<uint8[]> fileData;
			int64 fileSize;

			if (GetFileDataAndSize (path, fileData, fileSize))
			{
				result = StbImage::Import (fileData.get (), fileSize, true, false);

				if (bGenerateMipMaps && result.IsPowerOfTwo () && result.IsSquare ())
				{
					for (uint32 mipMapSize = result.m_array.at (0).m_mipMaps.at (0).m_width / 2; mipMapSize > 0; mipMapSize /= 2)
					{
						StbImage::AddAndGenerateMipMap (result, mipMapSize, mipMapSize);
					}
				}
			}
		}

		return result;
	}

	AudioData AssetImporter::ImportAudio (const PathString& path)
	{
		AudioData result;
		PathString extension = FileSystem::GetFileExtension (path);

		if (IsSupportedAudio (extension))
		{
			std::unique_ptr<uint8[]> fileData;
			int64 fileSize;

			if (GetFileDataAndSize (path, fileData, fileSize))
			{
				result = WavImporter::Import (fileData.get (), fileSize);
			}
		}

		return result;
	}

	bool AssetImporter::GetFileDataAndSize (const PathString& path, std::unique_ptr<uint8[]>& data, int64& size)
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

		data = std::make_unique<uint8[]> (fileSize);
		size = file.ReadAll (data.get ());

		return true;
	}
}
