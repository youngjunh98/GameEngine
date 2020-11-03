#include <vector>

#include "Rendering/Texture2D.h"
#include "Rendering/GlobalRenderer.h"

namespace GameEngine
{
	Texture2D::Texture2D ()
	{
	}

	Texture2D::~Texture2D ()
	{
	}

	void Texture2D::Destroy ()
	{
		Texture::Destroy ();
	}

	ETextureDimension Texture2D::Dimension () const
	{
		return ETextureDimension::Texture2D;
	}

	bool Texture2D::UpdateTextureResource (const std::vector<TextureResourceData>& resourceData)
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

		m_mipMapCount = resourceData.size ();
		m_texture2D = g_renderer.GetPlatformRenderingInterface ().CreateTexture2D (m_width, m_height, m_mipMapCount, 1, m_format, pixels.data (), pitches.data (), false, true, false, false);

		if (m_texture2D == nullptr)
		{
			return false;
		}

		m_srv = g_renderer.GetPlatformRenderingInterface ().CreateShaderResourceView (m_texture2D.get (), m_format, 0, 1, 0);

		if (m_srv == nullptr)
		{
			return false;
		}

		return true;
	}
}
