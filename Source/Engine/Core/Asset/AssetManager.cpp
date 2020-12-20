#include "AssetManager.h"
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

	void AssetManager::Shutdown ()
	{
		for (auto& assetInfo : GetInstance ().m_assetMap)
		{
			assetInfo.second.m_asset->Destroy ();
		}

		GetInstance ().m_assetMap.clear ();
	}

	void AssetManager::SaveAsset (const PathString& path)
	{
		auto* asset = FindAsset<Object> (path);

		if (asset == nullptr)
		{
			return;
		}

		File file (path, EFileAccessMode::Write);

		if (file.IsOpen ())
		{
			std::string assetData = Json::JsonSerializer::SerializeObject (*asset).dump ();
			file.Write (assetData.data (), assetData.size ());
		}
	}

	PathString AssetManager::GetAssetPath (const Object* asset)
	{
		PathString path;

		if (asset != nullptr)
		{
			for (auto& pair : m_assetMap)
			{
				if (asset == pair.second.m_asset.get ())
				{
					path = pair.first;
					break;
				}
			}
		}

		return path;
	}

	PathString AssetManager::GetAssetName (const Object* asset)
	{
		PathString name;

		if (asset != nullptr)
		{
			for (auto& pair : m_assetMap)
			{
				if (asset == pair.second.m_asset.get ())
				{
					name = FileSystem::GetFileName (pair.first);
					break;
				}
			}
		}

		return name;
	}

	void AssetManager::AddAsset (AssetInfo asset, const PathString& path)
	{
		if (asset.m_asset == nullptr)
		{
			return;
		}

		if (m_assetMap.find (path) == m_assetMap.end ())
		{
			m_assetMap.emplace (path, asset);
		}
		else
		{
			m_assetMap[path] = asset;
		}
	}

	std::vector<Object*> AssetManager::FindAssetsByType (EAssetType type)
	{
		std::vector<Object*> foundList;

		for (auto pair : m_assetMap)
		{
			EAssetType assetType = pair.second.m_type;
			Object* asset = pair.second.m_asset.get ();

			if (type == assetType)
			{
				foundList.push_back (asset);
			}
		}

		return foundList;
	}
}