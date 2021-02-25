#include <cstring>

#include "Texture.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/Asset/Image/StbImage.h"
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

	Texture::~Texture ()
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

	uint32 Texture::GetHeight () const
	{
		return m_height;
	}

	uint32 Texture::GetMipMapCount () const
	{
		return m_mipMapCount;
	}

	ERenderingResourceFormat Texture::GetFormat () const
	{
		return m_format;
	}

	void Texture::SetTextureData (const TextureData& textureData)
	{
		m_data = textureData;
	}

	bool Texture::UpdateTextureResource ()
	{
		bool bSucceed = false;

		if (m_data.m_array.size () > 0)
		{
			const auto& mipMaps = m_data.m_array.at (0).m_mipMaps;
			uint32 mipMapCount = static_cast<uint32> (mipMaps.size ());

			if (mipMapCount > 0)
			{
				if (m_data.m_format != ERenderingResourceFormat::None)
				{
					const TextureData::MipMapData& firstMipMap = mipMaps.at (0);

					if (firstMipMap.m_width > 0 && firstMipMap.m_height > 0)
					{
						m_width = firstMipMap.m_width;
						m_height = firstMipMap.m_height;
						m_mipMapCount = mipMapCount;
						m_format = m_data.m_format;

						bSucceed = true;
					}
				}
			}
		}

		return bSucceed;
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
		Json::JsonSerializer::CreateArray (json, "texture array");
		for (const TextureData::ArrayData& texture : m_data.m_array)
		{
			Json::Json arrayJson;
			Json::JsonSerializer::Serialize (arrayJson, "pixels", texture.m_fileData);
			Json::JsonSerializer::AppendArray (json, "texture array", arrayJson);
		}

		const auto& mipMaps = m_data.m_array.at (0).m_mipMaps;

		Json::JsonSerializer::CreateArray (json, "mipMaps");
		for (size_t i = 1; i < mipMaps.size (); i++)
		{
			Json::Json mipMapJson;
			Json::JsonSerializer::Serialize (mipMapJson, "width", mipMaps.at (i).m_width);
			Json::JsonSerializer::Serialize (mipMapJson, "height", mipMaps.at (i).m_height);
			Json::JsonSerializer::AppendArray (json, "mipMaps", mipMapJson);
		}

		Json::JsonSerializer::Serialize (json, "width", mipMaps.at (0).m_width);
		Json::JsonSerializer::Serialize (json, "height", mipMaps.at (0).m_height);
		Json::JsonSerializer::Serialize (json, "channels", m_data.m_channels);
		Json::JsonSerializer::Serialize (json, "bytes", m_data.m_bytes);
		Json::JsonSerializer::Serialize (json, "linear", m_data.m_bLinear);
		Json::JsonSerializer::Serialize (json, "format", static_cast<int32> (m_format));

		Json::JsonSerializer::Serialize<int32> (json, "filter", static_cast<int32> (m_filterMode));
		Json::JsonSerializer::Serialize<uint32> (json, "aniso", m_anisotropicLevel);
		Json::JsonSerializer::Serialize<int32> (json, "address", static_cast<int32> (m_addressMode));
	}

	void Texture::OnDeserialize (const Json::Json& json)
	{
		uint32 width = Json::JsonSerializer::Deserialize<uint32> (json, "width");
		uint32 height = Json::JsonSerializer::Deserialize<uint32> (json, "height");
		m_data.m_channels = Json::JsonSerializer::Deserialize<int32> (json, "channels");
		m_data.m_bytes = Json::JsonSerializer::Deserialize<int32> (json, "bytes");
		m_data.m_bLinear = Json::JsonSerializer::Deserialize<bool> (json, "linear");
		m_data.m_format = static_cast<ERenderingResourceFormat> (Json::JsonSerializer::Deserialize<int32> (json, "format"));

		m_data.m_array.clear ();

		for (Json::JsonConstIterator it = Json::JsonSerializer::GetArrayBegin (json, "texture array"); it != Json::JsonSerializer::GetArrayEnd (json, "texture array"); it++)
		{
			const Json::Json& arrayJson = it.value ();
			std::vector<uint8> pixels = Json::JsonSerializer::Deserialize<std::vector<uint8>> (arrayJson, "pixels");
			m_data.AddTextureArray (StbImage::Import (pixels.data (), pixels.size (), false, false));
		}

		for (Json::JsonConstIterator it = Json::JsonSerializer::GetArrayBegin (json, "mipMaps"); it != Json::JsonSerializer::GetArrayEnd (json, "mipMaps"); it++)
		{
			const Json::Json& mipMapJson = it.value ();
			int32 mipMapWidth = Json::JsonSerializer::Deserialize<int32> (mipMapJson, "width");
			int32 mipMapHeight = Json::JsonSerializer::Deserialize<int32> (mipMapJson, "height");
			StbImage::AddAndGenerateMipMap (m_data, mipMapWidth, mipMapHeight);
		}

		m_filterMode = static_cast<EFilterMode> (Json::JsonSerializer::Deserialize<int32> (json, "filter"));
		m_anisotropicLevel = Json::JsonSerializer::Deserialize<uint32> (json, "aniso");
		m_addressMode = static_cast<EAddressMode> (Json::JsonSerializer::Deserialize<int32> (json, "address"));

		UpdateTextureResource ();
		UpdateSamplerResource ();
	}
}
