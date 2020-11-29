#ifndef INCLUDE_ASSET_IMPORTER
#define INCLUDE_ASSET_IMPORTER

#include <memory>
#include <string>

#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/Texture2D.h"
#include "Engine/Rendering/TextureCube.h"
#include "Engine/Audio/AudioClip.h"

namespace GameEngine
{
	class AssetImporter
	{
	public:
		AssetImporter () {}
		virtual ~AssetImporter () = 0 {}

		static bool ImportAsset (const std::wstring& path);
		static void ImportAllAssets ();

		static std::shared_ptr<TextureCube> ImportTextureCube (const std::wstring cubePath[6]);

	private:
		static std::shared_ptr<Mesh> ImportMesh (const std::wstring& path);
		static std::shared_ptr<Texture2D> ImportTexture2D (const std::wstring& path, bool bGenerateMipMaps);
		static std::shared_ptr<AudioClip> ImportAudioClip (const std::wstring& path);

		static bool GetFileDataAndSize (const std::wstring& path, std::unique_ptr<int8[]>& data, int64& size);
	};
}

#endif