#pragma once

#include "TextureArray.h"

namespace GameEngine
{
	class TextureCube : public TextureArray
	{
	public:
		REGISTER_OBJECT_HEADER (TextureCube)

		TextureCube ();
		virtual ~TextureCube ();

		virtual void Destroy () override;

		virtual ETextureDimension Dimension () const override;
		virtual bool UpdateTextureResource () override;

	private:
		RenderingResourcePtr<RI_Texture2D> m_textureCube;
	};
}
