#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Engine/Core/File/FileSystem.h"
#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Rendering/Mesh.h"
#include "Engine/Engine/Rendering/Texture2D.h"
#include "Engine/Engine/Rendering/TextureCube.h"
#include "Engine/Engine/Audio/AudioClip.h"

namespace GameEngine
{
	class ENGINE_API AssetImporter
	{
	public:
		AssetImporter () {}
		virtual ~AssetImporter () = 0 {}

		static bool ImportAsset (const PathString& path, bool bInternalAsset);
		static std::shared_ptr<Shader> LoadShader (const PathString& path, bool bInternalAsset);
		static std::shared_ptr<TextureCube> ImportTextureCube (const PathString cubePath[6], bool bInternalAsset);

	private:
		static void RegisterAsset (const std::shared_ptr<Object>& asset, const PathString& path, bool bSaveAsset);

		static bool IsSupported3D (const PathString& extension);
		static bool IsSupported2D (const PathString& extension);
		static bool IsSupportedAudio (const PathString& extension);

		static std::shared_ptr<Mesh> Import3D (const PathString& path);
		static std::shared_ptr<Texture2D> Import2D (const PathString& path, bool bGenerateMipMaps);
		static std::shared_ptr<AudioClip> ImportAudio (const PathString& path);

		static bool GetFileDataAndSize (const PathString& path, std::unique_ptr<uint8[]>& data, int64& size);
	};
}
