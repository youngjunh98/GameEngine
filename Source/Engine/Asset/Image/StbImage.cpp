#include <stb_image.h>

#include "StbImage.h"

namespace GameEngine
{
	StbImage::StbImage () :
		m_data (nullptr),
		m_width (0),
		m_height (0),
		m_channels (0),
		m_byte (0),
		m_bLinear (true)
	{
	}

	StbImage::~StbImage ()
	{
		if (m_data != nullptr)
		{
			stbi_image_free (m_data);
		}

		m_data = nullptr;
		m_width = 0;
		m_height = 0;
		m_channels = 0;
		m_byte = 0;
	}

	bool StbImage::IsSquare () const
	{
		return (m_width == m_height) && (m_width > 0) && (m_height > 0);
	}

	bool StbImage::IsPowerOfTwo () const
	{
		return (m_width % 2 == 0) && (m_height % 2 == 0);
	}

	bool StbImage::IsGray () const
	{
		return (m_channels == STBI_grey) || (m_channels == STBI_grey_alpha);
	}

	bool StbImage::IsAlphaChannel () const
	{
		return (m_channels == STBI_grey_alpha) || (m_channels == STBI_rgb_alpha);
	}

	ERenderingResourceFormat StbImage::GetFormat () const
	{
		if (m_channels == 1)
		{
			if (m_byte == 1)
			{
				return m_bLinear ? ERenderingResourceFormat::R8_UNorm : ERenderingResourceFormat::R8_UNorm_SRGB;
			}
			else if (m_byte == 2)
			{
				return ERenderingResourceFormat::R16_UNorm;
			}
			else if (m_byte == 4)
			{
				return ERenderingResourceFormat::R32_Float;
			}
		}
		else if (m_channels == 2)
		{
			if (m_byte == 1)
			{
				return m_bLinear ? ERenderingResourceFormat::R8G8_UNorm : ERenderingResourceFormat::R8G8_UNorm_SRGB;
			}
			else if (m_byte == 2)
			{
				return ERenderingResourceFormat::R16G16_UNorm;
			}
			else if (m_byte == 4)
			{
				return ERenderingResourceFormat::R32G32_Float;
			}
		}
		else if (m_channels == 3)
		{
			if (m_byte == 1)
			{
				return m_bLinear ? ERenderingResourceFormat::R8G8B8_UNorm : ERenderingResourceFormat::R8G8B8_UNorm_SRGB;
			}
			else if (m_byte == 2)
			{
				return ERenderingResourceFormat::R16G16B16_UNorm;
			}
			else if (m_byte == 4)
			{
				return ERenderingResourceFormat::R32G32B32_Float;
			}
		}
		else if (m_channels == 4)
		{
			if (m_byte == 1)
			{
				return m_bLinear ? ERenderingResourceFormat::R8G8B8A8_UNorm : ERenderingResourceFormat::R8G8B8A8_UNorm_SRGB;
			}
			else if (m_byte == 2)
			{
				return ERenderingResourceFormat::R16G16B16A16_UNorm;
			}
			else if (m_byte == 4)
			{
				return ERenderingResourceFormat::R32G32B32A32_Float;
			}
		}

		return ERenderingResourceFormat::None;
	}

	int32 StbImage::GetRowPitch () const
	{
		return m_byte * m_width * m_channels;
	}

}