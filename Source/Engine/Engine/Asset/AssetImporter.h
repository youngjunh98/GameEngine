#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Engine/Core/File/FileSystem.h"
#include "Engine/Engine/Rendering/Mesh.h"
#include "Engine/Engine/Rendering/Texture2D.h"
#include "Engine/Engine/Rendering/TextureCube.h"
#include "Engine/Engine/Audio/AudioClip.h"

namespace GameEngine
{
	class AssetImporter
	{
	public:
		AssetImporter () {}
		virtual ~AssetImporter () = 0 {}

		static void ImportAllAssets ();
		static bool ImportAsset (const PathString& path);

		static std::shared_ptr<TextureCube> ImportTextureCube (const PathString cubePath[6]);

	private:
		static std::shared_ptr<Shader> LoadShader (const PathString& path);
		static std::shared_ptr<Mesh> ImportMesh (const PathString& path);
		static std::shared_ptr<Texture2D> ImportTexture2D (const PathString& path, bool bGenerateMipMaps);
		static std::shared_ptr<AudioClip> ImportAudioClip (const PathString& path);

		static bool GetFileDataAndSize (const PathString& path, std::unique_ptr<int8[]>& data, int64& size);
	};
}
