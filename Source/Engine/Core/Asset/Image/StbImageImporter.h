#pragma once

#include "StbImage.h"
#include "Engine/Core/CoreMacro.h"

namespace GameEngine
{
	class ENGINE_CORE_API StbImageImporter
	{
	public:
		static bool Import (StbImage& image, const int8* fileData, int64 fileSize, bool bForceAlphaChannel, bool bForceGray);
		static bool Resize (StbImage& resizedImage, const StbImage& sourceImage, int32 resizedWidth, int32 resizedHeight);
	};
}
