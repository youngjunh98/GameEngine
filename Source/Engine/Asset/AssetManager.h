#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "AssetMacro.h"
#include "Engine/Core/File/FileSystem.h"

namespace GameEngine
{
	class Object;

	class ENGINE_ASSET_API AssetManager final
	{
	private:
		AssetManager ();
		~AssetManager ();

		static AssetManager& GetInstance ();

	public:
		AssetManager (const AssetManager&) = delete;
		AssetManager& operator= (const AssetManager&) = delete;

		static bool IsAssetLoaded (const PathString& path);
		static void LoadAsset (const PathString& path);
		static void LoadAllAssets (const PathString& path);
		static void UnloadAsset (const PathString& path);
		static void UnloadAllAssets ();

		static void AddAsset (const std::shared_ptr<Object>& object, const PathString& path);
		static void SaveAsset (const PathString& path);
		static PathString ConvertToInternalAssetPath (const PathString& path);
		static std::shared_ptr<Object> GetAsset (const PathString& path);
		static std::vector<std::shared_ptr<Object>> GetAssets (const PathString& path);
		static std::vector<std::shared_ptr<Object>> FindAssets (const std::string& typeName);
		static PathString GetAssetPath (const Object& object);
		static PathString GetAssetName (const Object& object);

	private:
		static void LoadAllAssetsRecursive (const PathString& path);

	private:
		using AssetMap = std::unordered_map<PathString, std::shared_ptr<Object>>;
		AssetMap m_assetMap;
	};
}
