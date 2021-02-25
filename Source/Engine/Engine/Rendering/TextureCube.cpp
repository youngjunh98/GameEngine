#include "TextureCube.h"
#include "Engine/RI/RenderingInterface.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (TextureCube)

	TextureCube::TextureCube ()
	{
	}

	TextureCube::~TextureCube ()
	{
	}

	void TextureCube::Destroy ()
	{
		TextureArray::Destroy ();

		m_textureCube = nullptr;
	}

	ETextureDimension TextureCube::Dimension () const
	{
		return ETextureDimension::Texture2DArray;
	}

	bool TextureCube::UpdateTextureResource ()
	{
		if (TextureArray::UpdateTextureResource () == false)
		{
			return false;
		}

		if (m_arraySize != 6)
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

		void** dataPointer = reinterpret_cast<void**> (pixels.data ());
		uint32* rowBytesPointer = rowBytes.data ();
		RenderingInterface& renderingInterface = RenderingInterface::GetModule ();
		m_textureCube = renderingInterface.CreateTexture2D (m_width, m_height, m_mipMapCount, m_arraySize, m_format, dataPointer, rowBytesPointer, true, true, false, false);

		if (m_textureCube == nullptr)
		{
			return false;
		}

		m_srv = renderingInterface.CreateShaderResourceView (m_textureCube.get (), m_format, 0, 1, 0);

		if (m_srv == nullptr)
		{
			return false;
		}

		return true;
	}
}