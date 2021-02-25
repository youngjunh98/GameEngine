#pragma once

#include "Engine/Asset/AssetMacro.h"
#include "Engine/Asset/AssetData.h"
#include "Engine/RI/RenderingInterfaceEnum.h"

namespace GameEngine
{
	class ENGINE_ASSET_API StbImage
	{
	public:
		static TextureData Import (const uint8* fileData, int64 fileSize, bool bForceAlphaChannel, bool bForceGray);
		static bool GenerateMipMap (const TextureData::MipMapData& source, TextureData::MipMapData& destination,
			uint32 mipMapWidth, uint32 mipMapHeight, int32 channels, int32 bytes, bool bLinearColor);
		static bool AddAndGenerateMipMap (TextureData& texture, uint32 mipMapWidth, uint32 mipMapHeight);

		static bool IsRGB (const int32 channels);
		static bool IsAlpha (const int32 channels);
		static bool IsOpaque (const int32 channels);
		static ERenderingResourceFormat GetFormat (const int32 channels, const int32 bytes, const bool bLinearColor);
	};
}
