#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Engine/Object.h"
#include "Engine/Asset/AssetImporter.h"
#include "Engine/Rendering/Shader.h"
#include "Engine/Rendering/TextureCube.h"

namespace GameEngine
{
	class AssetManager
	{
	public:
		AssetManager ();
		~AssetManager ();

		void Shutdown ();

		TextureCube* FindTextureCubeAsset (const std::wstring& right, const std::wstring& left,
			const std::wstring& up, const std::wstring& down, const std::wstring& front, const std::wstring& back);

		Shader* LoadShader (const std::wstring& path);

		void SaveAsset (const std::wstring& path);
		std::wstring GetAssetPath (const Object* asset);
		std::wstring GetAssetName (const Object* asset);

		template<typename AssetType>
		AssetType* FindAsset (const std::wstring& path)
		{
			AssetType* asset = nullptr;
			auto assetIter = m_assetInfoMap.find (path);

			if (assetIter == m_assetInfoMap.end ())
			{
				if (AssetImporter::ImportAsset (path))
				{
					assetIter = m_assetInfoMap.find (path);
				}
			}

			if (assetIter != m_assetInfoMap.end ())
			{
				asset = dynamic_cast<AssetType*> (assetIter->second.m_asset.get ());
			}

			return asset;
		}

		template<typename AssetType>
		std::vector<AssetType*> FindAssets ()
		{
			std::vector<AssetType*> foundList;

			for (auto& info : m_assetInfoMap)
			{
				auto* asset = dynamic_cast<AssetType*> (info.second.m_asset.get ());

				if (asset != nullptr)
				{
					foundList.push_back (asset);
				}
			}

			return foundList;
		}

		void AddAsset (std::shared_ptr<Object> asset, const std::wstring& path);

	private:
		struct AssetInfo
		{
		public:
			std::wstring m_name;
			std::wstring m_directory;
			std::shared_ptr<Object> m_asset;
		};

		std::unordered_map<std::wstring, AssetInfo> m_assetInfoMap;
	};

	extern AssetManager g_assetManager;
}
