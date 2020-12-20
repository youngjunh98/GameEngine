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

	bool TextureCube::UpdateTextureResource (const std::vector<TextureResourceData>& resourceData)
	{
		if (m_width <= 0 || m_height <= 0)
		{
			return false;
		}

		std::vector<void*> pixels;
		std::vector<uint32> pitches;

		for (const auto& data : resourceData)
		{
			pixels.push_back (data.m_data);
			pitches.push_back (data.m_dataPitch);
		}

		RenderingInterface& renderingInterface = RenderingInterface::GetModule ();
		m_mipMapCount = static_cast<uint32> (resourceData.size () / 6);
		m_textureCube = renderingInterface.CreateTexture2D (m_width, m_height, m_mipMapCount, 6, m_format, pixels.data (), pitches.data (), true, true, false, false);

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