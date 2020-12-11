#include "Texture.h"
#include "Engine/Rendering/GlobalRenderer.h"

namespace GameEngine
{
	Texture::Texture () :
		m_width (0),
		m_height (0),
		m_mipMapCount (0),
		m_format (ERenderingResourceFormat::None),
		m_filterMode (EFilterMode::Point),
		m_anisotropicLevel (1),
		m_addressMode (EAddressMode::Wrap)
	{
	}

	void Texture::Destroy ()
	{
		Object::Destroy ();

		m_width = 0;
		m_height = 0;
		m_mipMapCount = 0;
		m_format = ERenderingResourceFormat::None;

		m_filterMode = EFilterMode::Point;
		m_anisotropicLevel = 1;
		m_addressMode = EAddressMode::Wrap;

		m_srv = nullptr;
		m_sampler = nullptr;
	}

	uint32 Texture::GetWidth () const
	{
		return m_width;
	}

	void Texture::SetWidth (uint32 width)
	{
		m_width = width;
	}

	uint32 Texture::GetHeight () const
	{
		return m_height;
	}

	void Texture::SetHeight (uint32 height)
	{
		m_height = height;
	}

	uint32 Texture::GetMipMapCount () const
	{
		return m_mipMapCount;
	}

	ERenderingResourceFormat Texture::GetFormat () const
	{
		return m_format;
	}

	void Texture::SetFormat (ERenderingResourceFormat format)
	{
		m_format = format;
	}

	EFilterMode Texture::GetFilterMode () const
	{
		return m_filterMode;
	}

	void Texture::SetFilterMode (EFilterMode filterMode)
	{
		m_filterMode = filterMode;
	}

	uint32 Texture::GetAnisotropicLevel () const
	{
		return m_anisotropicLevel;
	}

	void Texture::SetAnisotropicLevel (uint32 anisotropicLevel)
	{
		m_anisotropicLevel = Math::Clamp (anisotropicLevel, 1, 16);
	}

	EAddressMode Texture::GetAddressMode () const
	{
		return m_addressMode;
	}

	void Texture::SetAddressMode (EAddressMode addressMode)
	{
		m_addressMode = addressMode;
	}

	bool Texture::UpdateSamplerResource ()
	{
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_sampler = GlobalRenderer::GetRenderingInterface ().CreateSampler (m_addressMode, m_filterMode, m_anisotropicLevel, borderColor);

		if (m_sampler == nullptr)
		{
			return false;
		}

		return true;
	}

	RI_ShaderResourceView* Texture::GetSRV () const
	{
		return m_srv.get ();
	}

	RI_Sampler* Texture::GetSampler () const
	{
		return m_sampler.get ();
	}
}
