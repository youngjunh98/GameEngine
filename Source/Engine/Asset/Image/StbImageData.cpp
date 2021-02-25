#include <stb_image.h>

#include "StbImageData.h"

namespace GameEngine
{
	StbImageData::StbImageData () :
		m_data (nullptr)
	{
	}

	StbImageData::~StbImageData ()
	{
		if (m_data != nullptr)
		{
			stbi_image_free (m_data);
		}

		m_data = nullptr;
	}
}