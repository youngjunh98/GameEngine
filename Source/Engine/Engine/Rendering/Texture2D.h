#pragma once

#include "Texture.h"
#include "Engine/Engine/EngineMacro.h"

namespace GameEngine
{
	class ENGINE_API Texture2D : public Texture
	{
	public:
		REGISTER_OBJECT_HEADER (Texture2D)

		Texture2D ();
		virtual ~Texture2D ();

		virtual void Destroy () override;

		virtual ETextureDimension Dimension () const override;
		virtual bool UpdateTextureResource () override;

	protected:
		RenderingResourcePtr<RI_Texture2D> m_texture2D;
	};
}
