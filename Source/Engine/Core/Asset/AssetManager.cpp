#include "AssetManager.h"
#include "Engine/Platform/Platform.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/Core/File/File.h"

namespace GameEngine
{
	AssetManager::AssetManager ()
	{
	}

	AssetManager::~AssetManager ()
	{
	}

	AssetManager& AssetManager::GetInstance ()
	{
		static AssetManager instance;
		return instance;
	}

	bool AssetManager::IsAssetLoaded (const PathString& path)
	{
		AssetMap map = GetInstance ().m_assetMap;
		bool bLoaded = map.find (path) != map.end ();
		return bLoaded;
	}

	void AssetManager::LoadAsset (const PathString& path)
	{
		File file (path, EFileAccessMode::Read);

		if (file.IsOpen () == false)
		{
			return;
		}

		// Create buffer to read json.
		int64 fileSize = file.GetSize ();
		auto fileBuffer  = std::make_unique<uint8[]> (fileSize);

		// Read json from file.
		uint8* fileBufferStart = fileBuffer.get ();
		file.ReadAll (fileBufferStart);

		// Create asset object from json.
		Json::Json assetJson = Json::Json::parse (fileBufferStart, fileBufferStart + fileSize);
		std::shared_ptr<Object> asset = Json::JsonSerializer::JsonToObject (assetJson);

		AddAsset (asset, path);
	}

	void AssetManager::LoadAllAssets (const PathString& path)
	{
		LoadAllAssetsRecursive (path);
	}

	void AssetManager::UnloadAsset (const PathString& path)
	{
		if (IsAssetLoaded (path) == false)
		{
			return;
		}

		std::shared_ptr<Object> asset = GetAsset (path);
		AssetMap& map = GetInstance ().m_assetMap;
		map.erase (path);

		if (asset != nullptr)
		{
			if (asset->IsAlive ())
			{
				asset->Destroy ();
			}
		}
	}

	void AssetManager::UnloadAllAssets ()
	{
		AssetMap& map = GetInstance ().m_assetMap;

		for (auto& pair : map)
		{
			if (pair.second == nullptr)
			{
				continue;
			}

			Object& asset = *pair.second;
			asset.Destroy ();
		}

		map.clear ();
	}

	void AssetManager::AddAsset (const std::shared_ptr<Object>& object, const PathString& path)
	{
		if (object == nullptr)
		{
			return;
		}

		AssetMap& map = GetInstance ().m_assetMap;

		if (IsAssetLoaded (path))
		{
			// Destroy existing assset.
			map[path]->Destroy ();

			// Set new asset.
			map[path] = object;
		}
		else
		{
			map.emplace (path, object);
		}
	}

	void AssetManager::SaveAsset (const PathString& path)
	{
		std::shared_ptr<Object> asset = GetAsset (path);

		if (asset == nullptr)
		{
			return;
		}

		File file (path, EFileAccessMode::Write);

		if (file.IsOpen () == false)
		{
			return;
		}

		Json::Json assetJson = Json::JsonSerializer::ObjectToJson (*asset);
		std::string jsonString = assetJson.dump ();
		int64 jsonStringSize = jsonString.size ();
		file.Write (jsonString.data (), jsonStringSize);
	}

	PathString AssetManager::GetInternalAssetPath ()
	{
		PathString enginePath = FileSystem::RemoveFileName (Platform::GetGenericApplication ().GetExecutablePath ());
		PathString internalPath = FileSystem::CombinePath (enginePath, PATH ("Internal"));
		return internalPath;
	}

	std::shared_ptr<Object> AssetManager::GetInternalAsset (const PathString& path)
	{
		return GetAsset (FileSystem::CombinePath (GetInternalAssetPath (), path));
	}

	std::shared_ptr<Object> AssetManager::GetAsset (const PathString& path)
	{
		std::shared_ptr<Object> asset;

		if (IsAssetLoaded (path) == false && FileSystem::GetFileExtension (path) == PATH("asset"))
		{
			LoadAsset (path);
		}

		AssetMap& map = GetInstance ().m_assetMap;
		auto found = map.find (path);

		if (found != map.end ())
		{
			asset = found->second;

			// If asset is pending to destroy, erase it.
			if (asset->IsAlive () == false)
			{
				asset = nullptr;
				map.erase (path);
			}
		}


		return asset;
	}

	std::vector<std::shared_ptr<Object>> AssetManager::GetAssets (const PathString& path)
	{
		std::vector<std::shared_ptr<Object>> assets;
		AssetMap& map = GetInstance ().m_assetMap;
		PathString directory = FileSystem::RemoveFileName (path);
		bool bIgnorePath = path.size () <= 0;

		for (const auto& pair : map)
		{
			PathString assetPath = pair.first;
			PathString assetDirectory = FileSystem::RemoveFileName (assetPath);

			if (assetDirectory == directory || bIgnorePath)
			{
				std::shared_ptr<Object> asset = GetAsset (assetPath);

				if (asset != nullptr)
				{
					assets.push_back (asset);
				}
			}
		}

		return assets;
	}

	std::vector<std::shared_ptr<Object>> AssetManager::FindAssets (const std::string& typeName)
	{
		std::vector<std::shared_ptr<Object>> assets;

		for (const std::shared_ptr<Object>& asset : GetAssets (PATH("")))
		{
			if (asset->GetTypeName () == typeName)
			{
				assets.push_back (asset);
			}
		}

		return assets;
	}

	PathString AssetManager::GetAssetPath (const Object& object)
	{
		AssetMap& map = GetInstance ().m_assetMap;
		PathString path;

		for (const auto& pair : map)
		{
			const Object* assetPointer = pair.second.get ();

			if (assetPointer == &object)
			{
				path = pair.first;
				break;
			}
		}

		return path;
	}

	PathString AssetManager::GetAssetName (const Object& object)
	{
		AssetMap& map = GetInstance ().m_assetMap;
		PathString name;

		for (const auto& pair : map)
		{
			const Object* assetPointer = pair.second.get ();

			if (assetPointer == &object)
			{
				name = FileSystem::GetFileName (pair.first);
				break;
			}
		}

		return name;
	}

	void AssetManager::LoadAllAssetsRecursive (const PathString& path)
	{
		for (const PathString& fileName : FileSystem::GetFileNames (path))
		{
			PathString filePath = FileSystem::CombinePath (path, fileName);
			LoadAsset (filePath);
		}

		for (const PathString& directoryName : FileSystem::GetDirectoryNames (path))
		{
			PathString directoryPath = FileSystem::CombinePath (path, directoryName);
			LoadAllAssetsRecursive (directoryPath);
		}
	}
}