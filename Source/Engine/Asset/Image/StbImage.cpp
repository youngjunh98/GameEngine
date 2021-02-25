#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_resize.h>

#include "StbImage.h"
#include "StbImageData.h"

namespace GameEngine
{
	TextureData StbImage::Import (const uint8* fileData, int64 fileSize, bool bForceAlphaChannel, bool bForceGray)
	{
		TextureData texture;
		texture.m_channels = STBI_default;

		if (stbi_info_from_memory (fileData, fileSize, nullptr, nullptr, &texture.m_channels) == 1)
		{
			bool bNoSourceAlpha = IsOpaque (texture.m_channels);
			bool bIsRGB = IsRGB (texture.m_channels);

			if (bForceAlphaChannel && bNoSourceAlpha)
			{
				texture.m_channels += 1;
			}

			if (bForceGray && bIsRGB)
			{
				texture.m_channels -= 2;
			}

			StbImageData image;
			int32 imageWidth = 0;
			int32 imageHeight = 0;
			bool bIsHDR = stbi_is_hdr_from_memory (fileData, fileSize) == 1;
			bool bIs16Bit = stbi_is_16_bit_from_memory (fileData, fileSize) == 1;

			if (bIsHDR)
			{
				image.m_data = stbi_loadf_from_memory (fileData, fileSize, &imageWidth, &imageHeight, nullptr, texture.m_channels);
				texture.m_bytes = 4;
			}
			else if (bIs16Bit)
			{
				image.m_data = stbi_load_16_from_memory (fileData, fileSize, &imageWidth, &imageHeight, nullptr, texture.m_channels);
				texture.m_bytes = 2;
			}
			else
			{
				image.m_data = stbi_load_from_memory (fileData, fileSize, &imageWidth, &imageHeight, nullptr, texture.m_channels);
				texture.m_bytes = 1;
			}

			if (image.m_data != nullptr)
			{
				texture.m_format = GetFormat (texture.m_channels, texture.m_bytes, texture.m_bLinear);
				texture.m_array.emplace_back ();
				texture.m_array.back ().m_fileData = std::vector<uint8> (fileData, fileData + fileSize);
				texture.m_array.back ().m_mipMaps.emplace_back ();

				TextureData::MipMapData& original = texture.m_array.back ().m_mipMaps.back ();
				original.m_width = imageWidth;
				original.m_height = imageHeight;

				uint8* imageStart = static_cast<uint8*> (image.m_data);
				uint8* imageEnd = imageStart + texture.GetDataSizeInBytes (0, 0);
				original.m_pixels = std::vector<uint8> (imageStart, imageEnd);
			}
		}

		return texture;
	}

	bool StbImage::GenerateMipMap (const TextureData::MipMapData& source, TextureData::MipMapData& destination,
		uint32 mipMapWidth, uint32 mipMapHeight, int32 channels, int32 bytes, bool bLinearColor)
	{
		bool bSucceed = false;

		// Set size of mip map
		uint32 mipMapDataSize = mipMapWidth * mipMapHeight * channels * bytes;
		destination.m_pixels.resize (mipMapDataSize);

		// Stb params
		int32 alphaIndex = IsAlpha (channels) ? channels - 1 : STBIR_ALPHA_CHANNEL_NONE;
		int32 flags = 0;

		stbir_edge edge = STBIR_EDGE_WRAP;
		stbir_filter filter = STBIR_FILTER_BOX;
		stbir_colorspace colorSpace = bLinearColor ? STBIR_COLORSPACE_LINEAR : STBIR_COLORSPACE_SRGB;

		// Get data
		const uint8* sourceData = source.m_pixels.data ();
		uint8* mipMapData = destination.m_pixels.data ();

		if (bytes == 1)
		{
			bSucceed = stbir_resize_uint8_generic (sourceData, source.m_width, source.m_height, 0, mipMapData, mipMapWidth, mipMapHeight, 0,
				channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 1;
		}
		else if (bytes == 2)
		{
			auto* sourceData16 = reinterpret_cast<const uint16*> (sourceData);
			auto* mipMapData16 = reinterpret_cast<uint16*> (mipMapData);

			bSucceed = stbir_resize_uint16_generic (sourceData16, source.m_width, source.m_height, 0, mipMapData16, mipMapWidth, mipMapHeight, 0,
				channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 1;
		}
		else if (bytes == 4)
		{
			auto* sourceDataFloat = reinterpret_cast<const float*> (sourceData);
			auto* mipMapDataFloat = reinterpret_cast<float*> (mipMapData);

			bSucceed = stbir_resize_float_generic (sourceDataFloat, source.m_width, source.m_height, 0, mipMapDataFloat, mipMapWidth, mipMapHeight, 0,
				channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 1;
		}

		if (bSucceed)
		{
			destination.m_width = mipMapWidth;
			destination.m_height = mipMapHeight;
		}

		return bSucceed;
	}

	bool StbImage::AddAndGenerateMipMap (TextureData& texture, uint32 mipMapWidth, uint32 mipMapHeight)
	{
		bool bSucceed = false;

		if (texture.m_array.size () > 0)
		{
			for (TextureData::ArrayData& tex : texture.m_array)
			{
				tex.m_mipMaps.emplace_back ();
				TextureData::MipMapData& mipMap = tex.m_mipMaps.back ();
				const TextureData::MipMapData& first = tex.m_mipMaps.front ();

				if (GenerateMipMap (first, mipMap, mipMapWidth, mipMapHeight, texture.m_channels, texture.m_bytes, texture.m_bLinear))
				{
					bSucceed = true;
				}
			}
		}

		return bSucceed;
	}

	bool StbImage::IsRGB (const int32 channels)
	{
		return (channels == STBI_rgb) || (channels == STBI_rgb_alpha);
	}

	bool StbImage::IsAlpha (const int32 channels)
	{
		return (channels == STBI_grey_alpha) || (channels == STBI_rgb_alpha);
	}

	bool StbImage::IsOpaque (const int32 channels)
	{
		return (channels == STBI_grey) || (channels == STBI_rgb);
	}

	ERenderingResourceFormat StbImage::GetFormat (const int32 channels, const int32 bytes, const bool bLinearColor)
	{
		ERenderingResourceFormat format = ERenderingResourceFormat::None;

		if (channels == 1)
		{
			if (bytes == 1)
			{
				format = bLinearColor ? ERenderingResourceFormat::R8_UNorm : ERenderingResourceFormat::R8_UNorm_SRGB;
			}
			else if (bytes == 2)
			{
				format = ERenderingResourceFormat::R16_UNorm;
			}
			else if (bytes == 4)
			{
				format = ERenderingResourceFormat::R32_Float;
			}
		}
		else if (channels == 2)
		{
			if (bytes == 1)
			{
				format = bLinearColor ? ERenderingResourceFormat::R8G8_UNorm : ERenderingResourceFormat::R8G8_UNorm_SRGB;
			}
			else if (bytes == 2)
			{
				format = ERenderingResourceFormat::R16G16_UNorm;
			}
			else if (bytes == 4)
			{
				format = ERenderingResourceFormat::R32G32_Float;
			}
		}
		else if (channels == 3)
		{
			if (bytes == 1)
			{
				format = bLinearColor ? ERenderingResourceFormat::R8G8B8_UNorm : ERenderingResourceFormat::R8G8B8_UNorm_SRGB;
			}
			else if (bytes == 2)
			{
				format = ERenderingResourceFormat::R16G16B16_UNorm;
			}
			else if (bytes == 4)
			{
				format = ERenderingResourceFormat::R32G32B32_Float;
			}
		}
		else if (channels == 4)
		{
			if (bytes == 1)
			{
				format = bLinearColor ? ERenderingResourceFormat::R8G8B8A8_UNorm : ERenderingResourceFormat::R8G8B8A8_UNorm_SRGB;
			}
			else if (bytes == 2)
			{
				format = ERenderingResourceFormat::R16G16B16A16_UNorm;
			}
			else if (bytes == 4)
			{
				format = ERenderingResourceFormat::R32G32B32A32_Float;
			}
		}

		return format;
	}
}