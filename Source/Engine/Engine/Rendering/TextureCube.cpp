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
		Texture::Destroy ();

		m_textureCube = nullptr;
	}

	ETextureDimension TextureCube::Dimension () const
	{
		return ETextureDimension::Texture2DArray;
	}

	bool TextureCube::UpdateTextureResource ()
	{
		if (m_width <= 0 || m_height <= 0)
		{
			return false;
		}

		std::vector<uint8*> pixels;
		std::vector<uint32> rowBytes;

		for (TextureData& data : m_data)
		{
			pixels.push_back (data.m_data.data ());
			rowBytes.push_back (data.GetRowSizeInBytes ());
		}

		const uint32 arraySize = 6;
		m_mipMapCount = static_cast<uint32> (m_data.size () / arraySize);

		void** dataPointer = reinterpret_cast<void**> (pixels.data ());
		uint32* rowBytesPointer = rowBytes.data ();
		RenderingInterface& renderingInterface = RenderingInterface::GetModule ();
		m_textureCube = renderingInterface.CreateTexture2D (m_width, m_height, m_mipMapCount, arraySize, m_format, dataPointer, rowBytesPointer, true, true, false, false);

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