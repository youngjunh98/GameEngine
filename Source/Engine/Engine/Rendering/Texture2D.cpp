#include <vector>

#include "Texture2D.h"
#include "Engine/RI/RenderingInterface.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (Texture2D)

	Texture2D::Texture2D ()
	{
	}

	Texture2D::~Texture2D ()
	{
	}

	void Texture2D::Destroy ()
	{
		Texture::Destroy ();

		m_texture2D = nullptr;
	}

	ETextureDimension Texture2D::Dimension () const
	{
		return ETextureDimension::Texture2D;
	}

	bool Texture2D::UpdateTextureResource ()
	{
		if (Texture::UpdateTextureResource () == false)
		{
			return false;
		}

		std::vector<uint8*> pixels;
		std::vector<uint32> rowBytes;

		for (uint32 arrayIndex = 0; arrayIndex < m_data.m_array.size (); arrayIndex++)
		{
			TextureData::ArrayData& texture = m_data.m_array.at (arrayIndex);

			for (uint32 mipMapIndex = 0; mipMapIndex < texture.m_mipMaps.size (); mipMapIndex++)
			{
				TextureData::MipMapData& mipMap = texture.m_mipMaps.at (mipMapIndex);

				pixels.push_back (mipMap.m_pixels.data ());
				rowBytes.push_back (m_data.GetRowSizeInBytes (arrayIndex, mipMapIndex));
			}
		}

		RenderingInterface& renderingInterface = RenderingInterface::GetModule ();
		void** dataPointer = reinterpret_cast<void**> (pixels.data ());
		uint32* rowBytesPointer = rowBytes.data ();
		m_texture2D = renderingInterface.CreateTexture2D (m_width, m_height, m_mipMapCount, 1, m_format, dataPointer, rowBytesPointer, false, true, false, false);

		if (m_texture2D == nullptr)
		{
			return false;
		}

		m_srv = renderingInterface.CreateShaderResourceView (m_texture2D.get (), m_format, 0, 1, 0);

		if (m_srv == nullptr)
		{
			return false;
		}

		return true;
	}
}
