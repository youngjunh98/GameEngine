#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Engine/Core/CoreMacro.h"
#include "Engine/Core/Object.h"
#include "Engine/Core/File/FileSystem.h"

namespace GameEngine
{
	enum class EAssetType
	{
		None, Audio, Mesh, Texture, Shader, Material
	};

	struct AssetInfo
	{
		EAssetType m_type;
		std::shared_ptr<Object> m_asset;
	};

	class ENGINE_CORE_API AssetManager final
	{
	private:
		AssetManager ();
		~AssetManager ();

	public:
		AssetManager (const AssetManager&) = delete;
		AssetManager& operator= (const AssetManager&) = delete;

		static AssetManager& GetInstance ();

		void Shutdown ();

		void SaveAsset (const PathString& path);
		PathString GetAssetPath (const Object* asset);
		PathString GetAssetName (const Object* asset);

		template<typename TAsset>
		TAsset* FindAsset (const PathString& path)
		{
			TAsset* found = nullptr;
			auto pair = m_assetMap.find (path);

			if (pair != m_assetMap.end ())
			{
				Object* asset = pair->second.m_asset.get ();
				found = dynamic_cast<TAsset*> (asset);
			}

			return found;
		}

		template<typename TAsset>
		std::vector<TAsset*> FindAssets ()
		{
			std::vector<TAsset*> foundList;

			for (auto pair : m_assetMap)
			{
				Object* asset = pair.second.m_asset.get ();
				TAsset* casted = dynamic_cast<TAsset*> (asset);

				if (casted != nullptr)
				{
					foundList.push_back (casted);
				}
			}

			return foundList;
		}

		void AddAsset (AssetInfo asset, const PathString& path);
		std::vector<Object*> FindAssetsByType (EAssetType type);

	private:
		std::unordered_map<PathString, AssetInfo> m_assetMap;
	};
}
