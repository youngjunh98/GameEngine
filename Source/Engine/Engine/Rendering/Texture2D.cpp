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

		m_mipMapCount = m_data.size ();

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
