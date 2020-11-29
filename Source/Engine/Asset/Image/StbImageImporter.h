#ifndef INCLUDE_STB_IMAGE_IMPORTER
#define INCLUDE_STB_IMAGE_IMPORTER

#include "StbImage.h"

namespace GameEngine
{
	class StbImageImporter
	{
	public:
		static bool Import (StbImage& image, const int8* fileData, int64 fileSize, bool bForceAlphaChannel, bool bForceGray);
		static bool Resize (StbImage& resizedImage, const StbImage& sourceImage, int32 resizedWidth, int32 resizedHeight);
	};
}

#endif