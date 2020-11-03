#ifndef INCLUDE_TEXTURE_CUBE
#define INCLUDE_TEXTURE_CUBE

#include "Rendering/Texture.h"

namespace GameEngine
{
	class TextureCube : public Texture
	{
	public:
		TextureCube ();
		virtual ~TextureCube ();

		virtual void Destroy () override;

		virtual ETextureDimension Dimension () const override;
		virtual bool UpdateTextureResource (const std::vector<TextureResourceData>& resourceData) override;

	private:
		RenderingResourcePtr<RI_Texture2D> m_textureCube;
	};
}

#endif