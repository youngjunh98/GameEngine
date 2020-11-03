#ifndef INCLUDE_ASSET_IMPORTER
#define INCLUDE_ASSET_IMPORTER

#include <memory>
#include <string>

#include "Rendering/Mesh.h"
#include "Rendering/Texture2D.h"
#include "Rendering/TextureCube.h"
#include "Sound/SoundClip.h"

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

		static std::shared_ptr<SoundClip> ImportSoundClip (const std::wstring& path);

		static bool GetFileDataAndSize (const std::wstring& path, std::unique_ptr<int8[]>& data, int64& size);
		static std::wstring GetFileExtension (const std::wstring& path);
	};
}

#endif