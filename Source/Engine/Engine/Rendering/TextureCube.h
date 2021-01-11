#pragma once

#include "Texture.h"

namespace GameEngine
{
	class TextureCube : public Texture
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
