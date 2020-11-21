#include "Asset/AssetManager.h"
#include "Core/JSON/JsonSerializer.h"
#include "Core/File/File.h"

namespace GameEngine
{
	AssetManager g_assetManager;

	AssetManager::AssetManager ()
	{
	}

	AssetManager::~AssetManager ()
	{
	}

	void AssetManager::Shutdown ()
	{
		for (auto& assetInfo : g_assetManager.m_assetInfoMap)
		{
			assetInfo.second.m_asset->Destroy ();
			assetInfo.second.m_asset = nullptr;
		}

		g_assetManager.m_assetInfoMap.clear ();
	}

	TextureCube*  AssetManager::FindTextureCubeAsset (const std::wstring& right, const std::wstring& left,
		const std::wstring& up, const std::wstring& down, const std::wstring& front, const std::wstring& back)
	{
		TextureCube* textureCubeAsset = nullptr;
		auto assetIter = m_assetInfoMap.find (right);

		if (assetIter != m_assetInfoMap.end ())
		{
			textureCubeAsset = reinterpret_cast<TextureCube*> (assetIter->second.m_asset.get());
		}

		if (textureCubeAsset == nullptr)
		{
			const std::wstring cubePath[6] = { right, left, up, down, front, back };
			auto textureCube = AssetImporter::ImportTextureCube (cubePath);

			if (textureCube == nullptr)
			{
				return nullptr;
			}

			textureCubeAsset = textureCube.get ();
		}

		return textureCubeAsset;
	}

	Shader* AssetManager::LoadShader (const std::wstring& path)
	{
		Shader* shaderAsset = FindAsset<Shader> (path);

		if (shaderAsset == nullptr)
		{
			auto shader = std::make_shared<Shader> ();

			if (shader->Initialize (path) == false ||
				(shader->GetVertexShader () == nullptr &&
				shader->GetPixelShader () == nullptr &&
				shader->GetDomainShader () == nullptr &&
				shader->GetHullShader () == nullptr &&
				shader->GetGeometryShader () == nullptr))
			{
				return nullptr;
			}

			AddAsset (shader, path);
			shaderAsset = shader.get ();
		}

		return shaderAsset;
	}

	void AssetManager::SaveAsset (const std::wstring& path)
	{
		auto* asset = FindAsset<Object> (path);

		if (asset == nullptr)
		{
			return;
		}

		File file (path, EFileAccessMode::Write);

		if (file.IsOpen ())
		{
			std::string assetData = Json::JsonSerializer::Serialize (*asset).dump ();
			file.Write (assetData.data (), assetData.size ());
		}
	}

	std::wstring AssetManager::GetAssetPath (const Object* asset)
	{
		std::wstring path;

		if (asset != nullptr)
		{
			for (auto& info : m_assetInfoMap)
			{
				if (asset == info.second.m_asset.get ())
				{
					path = info.second.m_directory + PATH ("/") + info.second.m_name;
					break;
				}
			}
		}

		return path;
	}

	std::wstring AssetManager::GetAssetName (const Object* asset)
	{
		std::wstring name;

		if (asset != nullptr)
		{
			for (auto& info : m_assetInfoMap)
			{
				if (asset == info.second.m_asset.get ())
				{
					name = info.second.m_name;
					break;
				}
			}
		}

		return name;
	}

	void AssetManager::AddAsset (std::shared_ptr<Object> asset, const std::wstring& path)
	{
		if (asset == nullptr)
		{
			return;
		}

		auto directoryEnd = path.find_last_of (L'/');

		AssetInfo info;
		info.m_name = path.substr (directoryEnd + 1);
		info.m_directory = path.substr (0, directoryEnd);
		info.m_asset = asset;

		if (m_assetInfoMap.find (path) == m_assetInfoMap.end ())
		{
			m_assetInfoMap.emplace (path, info);
		}
		else
		{
			m_assetInfoMap[path] = info;
		}
	}
}