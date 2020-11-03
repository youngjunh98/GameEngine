#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_resize.h>

#include "Asset/Image/StbImageImporter.h"
#include "Asset/Image/StbImage.h"

namespace GameEngine
{
	bool StbImageImporter::Import (StbImage& image, const int8* fileData, int64 fileSize, bool bForceAlphaChannel, bool bForceGray)
	{
		int32 channels = STBI_default;

		auto data = reinterpret_cast<const uint8*> (fileData);
		auto dataSize = static_cast<int32> (fileSize);

		if (stbi_info_from_memory (data, dataSize, nullptr, nullptr, &channels) == 0)
		{
			return false;
		}

		bool bNoSourceAlpha = channels == STBI_grey || channels == STBI_rgb;
		bool bIsRGB = channels == STBI_rgb || channels == STBI_rgb_alpha;

		if (bForceAlphaChannel && bNoSourceAlpha)
		{
			channels += 1;
		}

		if (bForceGray && bIsRGB)
		{
			channels -= 2;
		}

		image.m_channels = channels;

		bool bIsHDR = stbi_is_hdr_from_memory (data, dataSize) == 1;
		bool bIs16Bit = stbi_is_16_bit_from_memory (data, dataSize) == 1;

		if (bIsHDR)
		{
			image.m_data = stbi_loadf_from_memory (data, dataSize, &image.m_width, &image.m_height, nullptr, channels);
			image.m_byte = 4;
		}
		else if (bIs16Bit)
		{
			image.m_data = stbi_load_16_from_memory (data, dataSize, &image.m_width, &image.m_height, nullptr, channels);
			image.m_byte = 2;
		}
		else
		{
			image.m_data = stbi_load_from_memory (data, dataSize, &image.m_width, &image.m_height, nullptr, channels);
			image.m_byte = 1;
		}

		if (image.m_data == nullptr)
		{
			return false;
		}

		return true;
	}

	bool StbImageImporter::Resize (StbImage& resizedImage, const StbImage& sourceImage, int32 resizedWidth, int32 resizedHeight)
	{
		resizedImage.m_data = STBI_MALLOC (sourceImage.m_byte * sourceImage.m_width * sourceImage.m_height * sourceImage.m_channels);
		resizedImage.m_width = resizedWidth;
		resizedImage.m_height = resizedHeight;
		resizedImage.m_channels = sourceImage.m_channels;
		resizedImage.m_byte = sourceImage.m_byte;

		int32 alphaIndex = sourceImage.IsAlphaChannel () ? sourceImage.m_channels - 1 : STBIR_ALPHA_CHANNEL_NONE;
		int32 flags = 0;

		stbir_edge edge = STBIR_EDGE_WRAP;
		stbir_filter filter = STBIR_FILTER_BOX;
		stbir_colorspace colorSpace = sourceImage.m_bLinear ? STBIR_COLORSPACE_LINEAR : STBIR_COLORSPACE_SRGB;

		if (sourceImage.m_byte == 1)
		{
			auto* sourceData = static_cast<uint8*> (sourceImage.m_data);
			auto* resizedData = static_cast<uint8*> (resizedImage.m_data);

			if (stbir_resize_uint8_generic (sourceData, sourceImage.m_width, sourceImage.m_height, 0, resizedData, resizedWidth, resizedHeight, 0,
				sourceImage.m_channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 0)
			{
				return false;
			}
		}
		else if (sourceImage.m_byte == 2)
		{
			auto* sourceData = static_cast<uint16*> (sourceImage.m_data);
			auto* resizedData = static_cast<uint16*> (resizedImage.m_data);

			if (stbir_resize_uint16_generic (sourceData, sourceImage.m_width, sourceImage.m_height, 0, resizedData, resizedWidth, resizedHeight, 0,
				sourceImage.m_channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 0)
			{
				return false;
			}
		}
		else if (sourceImage.m_byte == 4)
		{
			auto* sourceData = static_cast<float*> (sourceImage.m_data);
			auto* resizedData = static_cast<float*> (resizedImage.m_data);

			if (stbir_resize_float_generic (sourceData, sourceImage.m_width, sourceImage.m_height, 0, resizedData, resizedWidth, resizedHeight, 0,
				sourceImage.m_channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 0)
			{
				return false;
			}
		}

		return true;
	}
}