#pragma once

#include "Engine/RI/RenderingInterfaceEnum.h"
#include "Engine/Engine/Asset/AssetData.h"

namespace GameEngine
{
	class StbImage
	{
	public:
		static TextureData Import (const uint8* fileData, int64 fileSize, bool bForceAlphaChannel, bool bForceGray);
		static TextureData Resize (const TextureData& textureData,  int32 resizedWidth, int32 resizedHeight);

		static bool IsRGB (const int32 channels);
		static bool IsAlpha (const int32 channels);
		static bool IsOpaque (const int32 channels);
		static ERenderingResourceFormat GetFormat (const int32 channels, const int32 bytes, const bool bLinearColor);
	};
}
