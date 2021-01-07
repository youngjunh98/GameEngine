#include <cstring>

#include "Texture.h"
#include "Engine/Core/JSON/JsonSerializer.h"
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
		json["texture data"] = Json::Json::array ();

		for (const TextureData& data : m_data)
		{
			Json::Json dataJson;
			Json::JsonSerializer::Serialize<std::vector<uint8>> (dataJson, "data", data.m_data);
			Json::JsonSerializer::Serialize<uint32> (dataJson, "size", data.m_dataBytes);
			Json::JsonSerializer::Serialize<uint32> (dataJson, "row size", data.m_dataRowBytes);
			Json::JsonSerializer::Serialize<uint32> (dataJson, "square size", data.m_dataSquareBytes);
			json["texture data"].push_back (dataJson);
		}

		Json::JsonSerializer::Serialize<uint32> (json, "width", m_width);
		Json::JsonSerializer::Serialize<uint32> (json, "height", m_height);
		Json::JsonSerializer::Serialize<uint32> (json, "mipmap", m_mipMapCount);
		Json::JsonSerializer::Serialize<int32> (json, "format", static_cast<int32> (m_format));

		Json::JsonSerializer::Serialize<int32> (json, "filter", static_cast<int32> (m_filterMode));
		Json::JsonSerializer::Serialize<uint32> (json, "aniso", m_anisotropicLevel);
		Json::JsonSerializer::Serialize<int32> (json, "address", static_cast<int32> (m_addressMode));
	}

	void Texture::OnDeserialize (const Json::Json& json)
	{
		if (json.contains ("texture data"))
		{
			m_data.clear ();

			for (const Json::Json& dataJson : json["texture data"])
			{
				TextureData data;
				data.m_data = Json::JsonSerializer::Deserialize<std::vector<uint8>> (dataJson, "data");
				data.m_dataBytes = Json::JsonSerializer::Deserialize<uint32> (dataJson, "size");
				data.m_dataRowBytes = Json::JsonSerializer::Deserialize<uint32> (dataJson, "row size");
				data.m_dataSquareBytes = Json::JsonSerializer::Deserialize<uint32> (dataJson, "square size");

				m_data.push_back (data);
			}
		}

		m_width = Json::JsonSerializer::Deserialize<uint32> (json, "width");
		m_height = Json::JsonSerializer::Deserialize<uint32> (json, "height");
		m_mipMapCount = Json::JsonSerializer::Deserialize<uint32> (json, "mipmap");
		m_format = static_cast<ERenderingResourceFormat> (Json::JsonSerializer::Deserialize<int32> (json, "format"));

		m_filterMode = static_cast<EFilterMode> (Json::JsonSerializer::Deserialize<int32> (json, "filter"));
		m_anisotropicLevel = Json::JsonSerializer::Deserialize<uint32> (json, "aniso");
		m_addressMode = static_cast<EAddressMode> (Json::JsonSerializer::Deserialize<int32> (json, "address"));

		UpdateTextureResource ();
		UpdateSamplerResource ();
	}
}
