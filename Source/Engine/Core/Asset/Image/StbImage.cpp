#include <stb_image.h>

#include "StbImage.h"

namespace GameEngine
{
	StbImage::StbImage () :
		m_data (nullptr)
	{
	}

	StbImage::~StbImage ()
	{
		if (m_data != nullptr)
		{
			stbi_image_free (m_data);
		}

		m_data = nullptr;
	}
}