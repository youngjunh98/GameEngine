#pragma once

#include <vector>
#include <memory>

#include "Type.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/RI/RenderingInterfaceResource.h"
#include "Engine/RI/RenderingInterfaceEnum.h"
#include "Engine/Asset/AssetData.h"
#include "Engine/Engine/EngineMacro.h"

namespace GameEngine
{
	enum class ETextureDimension
	{
		Texture1D,
		Texture1DArray,
		Texture2D,
		Texture2DArray,
		Texture3D
	};

	class ENGINE_API Texture : public Object
	{
	public:
		Texture ();
		virtual ~Texture () = 0;

		virtual void Destroy () override;

		virtual ETextureDimension Dimension () const = 0;
		uint32 GetWidth () const;
		uint32 GetHeight () const;
		uint32 GetMipMapCount () const;
		ERenderingResourceFormat GetFormat () const;

		void SetTextureData (const TextureData& textureData);
		virtual bool UpdateTextureResource ();

		EFilterMode GetFilterMode () const;
		void SetFilterMode (EFilterMode filterMode);

		uint32 GetAnisotropicLevel () const;
		void SetAnisotropicLevel (uint32 anisotropicLevel);

		EAddressMode GetAddressMode () const;
		void SetAddressMode (EAddressMode addressMode);

		bool UpdateSamplerResource ();

		RI_ShaderResourceView* GetTextureResource () const;
		RI_Sampler* GetSampler () const;

		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	protected:
		TextureData m_data;

		uint32 m_width;
		uint32 m_height;
		uint32 m_mipMapCount;
		ERenderingResourceFormat m_format;

		EFilterMode m_filterMode;
		uint32 m_anisotropicLevel;
		EAddressMode m_addressMode;

		RenderingResourcePtr<RI_ShaderResourceView> m_srv;
		RenderingResourcePtr<RI_Sampler> m_sampler;
	};
}
