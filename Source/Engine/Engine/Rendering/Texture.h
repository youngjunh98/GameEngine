#ifndef INCLUDE_TEXTURE
#define INCLUDE_TEXTURE

#include <vector>

#include "Engine/Core/CoreMinimal.h"
#include "Engine/RI/RenderingInterfaceResource.h"
#include "Engine/RI/RenderingInterfaceEnum.h"
#include "Engine/Core/Object.h"

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

	struct TextureResourceData
	{
		void* m_data;
		uint32 m_dataPitch;
		uint32 m_dataSlicePitch;
	};

	class Texture : public Object
	{
	public:
		Texture ();
		virtual ~Texture () = 0 {}

		virtual void Destroy () override;

		virtual ETextureDimension Dimension () const = 0;

		virtual uint32 GetWidth () const;
		virtual void SetWidth (uint32 width);

		virtual uint32 GetHeight () const;
		virtual void SetHeight (uint32 height);

		virtual uint32 GetMipMapCount () const;

		virtual ERenderingResourceFormat GetFormat () const;
		virtual void SetFormat (ERenderingResourceFormat format);

		virtual bool UpdateTextureResource (const std::vector<TextureResourceData>& resourceData) = 0;

		EFilterMode GetFilterMode () const;
		void SetFilterMode (EFilterMode filterMode);

		uint32 GetAnisotropicLevel () const;
		void SetAnisotropicLevel (uint32 anisotropicLevel);

		EAddressMode GetAddressMode () const;
		void SetAddressMode (EAddressMode addressMode);

		bool UpdateSamplerResource ();

		RI_ShaderResourceView* GetSRV () const;
		RI_Sampler* GetSampler () const;

	protected:
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

#endif