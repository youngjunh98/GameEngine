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
			bool bIsHDR = stbi_is_hdr_from_memory (fileData, fileSize) == 1;
			bool bIs16Bit = stbi_is_16_bit_from_memory (fileData, fileSize) == 1;

			if (bIsHDR)
			{
				image.m_data = stbi_loadf_from_memory (fileData, fileSize, &texture.m_width, &texture.m_height, nullptr, texture.m_channels);
				texture.m_bytes = 4;
			}
			else if (bIs16Bit)
			{
				image.m_data = stbi_load_16_from_memory (fileData, fileSize, &texture.m_width, &texture.m_height, nullptr, texture.m_channels);
				texture.m_bytes = 2;
			}
			else
			{
				image.m_data = stbi_load_from_memory (fileData, fileSize, &texture.m_width, &texture.m_height, nullptr, texture.m_channels);
				texture.m_bytes = 1;
			}

			if (image.m_data != nullptr)
			{
				uint8* imageStart = static_cast<uint8*> (image.m_data);
				uint8* imageEnd = imageStart + texture.GetDataSizeInBytes ();
				texture.m_data = std::vector<uint8> (imageStart, imageEnd);
			}
		}

		return texture;
	}

	TextureData StbImage::Resize (const TextureData& textureData, int32 resizedWidth, int32 resizedHeight)
	{
		bool bSucceed = false;

		TextureData resized;
		int32 resizedDataSize = resizedWidth * resizedHeight * textureData.m_channels * textureData.m_bytes;
		resized.m_data.resize (resizedDataSize);

		int32 alphaIndex = IsAlpha (textureData.m_channels) ? textureData.m_channels - 1 : STBIR_ALPHA_CHANNEL_NONE;
		int32 flags = 0;

		stbir_edge edge = STBIR_EDGE_WRAP;
		stbir_filter filter = STBIR_FILTER_BOX;
		stbir_colorspace colorSpace = textureData.m_bLinear ? STBIR_COLORSPACE_LINEAR : STBIR_COLORSPACE_SRGB;

		if (textureData.m_bytes == 1)
		{
			auto* sourceData = static_cast<const uint8*> (textureData.m_data.data ());
			auto* resizedData = static_cast<uint8*> (resized.m_data.data ());

			bSucceed = stbir_resize_uint8_generic (sourceData, textureData.m_width, textureData.m_height, 0, resizedData, resizedWidth, resizedHeight, 0,
				textureData.m_channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 1;
		}
		else if (textureData.m_bytes == 2)
		{
			auto* sourceData = reinterpret_cast<const uint16*> (textureData.m_data.data ());
			auto* resizedData = reinterpret_cast<uint16*> (resized.m_data.data ());

			bSucceed = stbir_resize_uint16_generic (sourceData, textureData.m_width, textureData.m_height, 0, resizedData, resizedWidth, resizedHeight, 0,
				textureData.m_channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 1;
		}
		else if (textureData.m_bytes == 4)
		{
			auto* sourceData = reinterpret_cast<const float*> (textureData.m_data.data ());
			auto* resizedData = reinterpret_cast<float*> (resized.m_data.data ());

			bSucceed = stbir_resize_float_generic (sourceData, textureData.m_width, textureData.m_height, 0, resizedData, resizedWidth, resizedHeight, 0,
				textureData.m_channels, alphaIndex, flags, edge, filter, colorSpace, nullptr) == 1;
		}

		if (bSucceed)
		{
			resized.m_width = resizedWidth;
			resized.m_height = resizedHeight;
			resized.m_channels = textureData.m_channels;
			resized.m_bytes = textureData.m_bytes;
			resized.m_bLinear = textureData.m_bLinear;
		}
		else
		{
			resized.m_data.clear ();
		}

		return resized;
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