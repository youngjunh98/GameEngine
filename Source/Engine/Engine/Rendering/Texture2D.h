#pragma once

#include "Texture.h"

namespace GameEngine
{
	class Texture2D : public Texture
	{
	public:
		Texture2D ();
		virtual ~Texture2D ();

		virtual void Destroy () override;

		virtual ETextureDimension Dimension () const override;
		virtual bool UpdateTextureResource (const std::vector<TextureResourceData>& resourceData) override;

	protected:
		RenderingResourcePtr<RI_Texture2D> m_texture2D;
	};
}
