#pragma once

#include <vector>
#include <memory>
#include <string>

#include "AssetMacro.h"
#include "AssetData.h"
#include "Engine/Core/File/FileSystem.h"

namespace GameEngine
{
	class ENGINE_ASSET_API AssetImporter
	{
	public:
		AssetImporter ();
		virtual ~AssetImporter () = 0;

		static bool IsSupportedMesh (const PathString& fileExtension);
		static bool IsSupportedTexture (const PathString& fileExtension);
		static bool IsSupportedAudio (const PathString& fileExtension);

		static MeshData ImportMesh (const PathString& path);
		static TextureData ImportTexture (const PathString& path, bool bGenerateMipMaps);
		static AudioData ImportAudio (const PathString& path);

	private:
		static bool GetFileDataAndSize (const PathString& path, std::unique_ptr<uint8[]>& data, int64& size);
	};
}
