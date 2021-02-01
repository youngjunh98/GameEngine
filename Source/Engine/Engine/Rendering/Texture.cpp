#include <cstring>

#include "Texture.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/Core/Asset/Image/StbImageImporter.h"
#include "Engine/RI/RenderingInterface.h"

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

	void Texture::SetTextureData (const std::vector<TextureData>& textureData)
	{
		m_data.clear ();
		m_data = textureData;
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
		m_sampler = RenderingInterface::GetModule ().CreateSampler (m_addressMode, m_filterMode, m_anisotropicLevel, borderColor);

		if (m_sampler == nullptr)
		{
			return false;
		}

		return true;
	}

	RI_ShaderResourceView* Texture::GetTextureResource () const
	{
		return m_srv.get ();
	}

	RI_Sampler* Texture::GetSampler () const
	{
		return m_sampler.get ();
	}

	void Texture::OnSerialize (Json::Json& json) const
	{
		Json::JsonSerializer::Serialize (json, "format", static_cast<int32> (m_format));
		Json::JsonSerializer::Serialize (json, "width", m_data.at (0).m_width);
		Json::JsonSerializer::Serialize (json, "height", m_data.at (0).m_height);
		Json::JsonSerializer::Serialize (json, "pixels", m_data.at (0).m_data);
		Json::JsonSerializer::Serialize (json, "channels", m_data.at (0).m_channels);
		Json::JsonSerializer::Serialize (json, "bytes", m_data.at (0).m_bytes);
		Json::JsonSerializer::Serialize (json, "linear", m_data.at (0).m_bLinear);

		Json::JsonSerializer::CreateArray (json, "mipMaps");

		for (size_t i = 1; i < m_data.size (); i++)
		{
			Json::Json mipMapJson;
			Json::JsonSerializer::Serialize (mipMapJson, "width", m_data.at (i).m_width);
			Json::JsonSerializer::Serialize (mipMapJson, "height", m_data.at (i).m_height);
			Json::JsonSerializer::AppendArray (json, "mipMaps", mipMapJson);
		}

		Json::JsonSerializer::Serialize<int32> (json, "filter", static_cast<int32> (m_filterMode));
		Json::JsonSerializer::Serialize<uint32> (json, "aniso", m_anisotropicLevel);
		Json::JsonSerializer::Serialize<int32> (json, "address", static_cast<int32> (m_addressMode));
	}

	void Texture::OnDeserialize (const Json::Json& json)
	{
		m_format = static_cast<ERenderingResourceFormat> (Json::JsonSerializer::Deserialize<int32> (json, "format"));

		TextureData texture;
		texture.m_width = Json::JsonSerializer::Deserialize<int32> (json, "width");
		texture.m_height = Json::JsonSerializer::Deserialize<int32> (json, "height");
		texture.m_data = Json::JsonSerializer::Deserialize<std::vector<uint8>> (json, "pixels");
		texture.m_channels = Json::JsonSerializer::Deserialize<int32> (json, "channels");
		texture.m_bytes = Json::JsonSerializer::Deserialize<int32> (json, "bytes");
		texture.m_bLinear = Json::JsonSerializer::Deserialize<bool> (json, "linear");

		m_width = static_cast<uint32> (texture.m_width);
		m_height = static_cast<uint32> (texture.m_height);

		m_data.clear ();
		m_data.push_back (texture);

		for (Json::JsonConstIterator it = Json::JsonSerializer::GetArrayBegin (json, "mipMaps"); it != Json::JsonSerializer::GetArrayEnd (json, "mipMaps"); it++)
		{
			const Json::Json& mipMapJson = it.value ();
			int32 mipMapWidth = Json::JsonSerializer::Deserialize<int32> (mipMapJson, "width");
			int32 mipMapHeight = Json::JsonSerializer::Deserialize<int32> (mipMapJson, "height");
			TextureData mipMap = StbImageImporter::Resize (texture, mipMapWidth, mipMapHeight);

			if (mipMap.GetDataSizeInBytes () > 0)
			{
				m_data.push_back (mipMap);
			}
		}

		m_filterMode = static_cast<EFilterMode> (Json::JsonSerializer::Deserialize<int32> (json, "filter"));
		m_anisotropicLevel = Json::JsonSerializer::Deserialize<uint32> (json, "aniso");
		m_addressMode = static_cast<EAddressMode> (Json::JsonSerializer::Deserialize<int32> (json, "address"));

		UpdateTextureResource ();
		UpdateSamplerResource ();
	}
}
