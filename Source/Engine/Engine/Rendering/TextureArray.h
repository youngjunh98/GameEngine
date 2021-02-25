#pragma once

#include <vector>
#include <memory>

#include "Type.h"
#include "Texture.h"

namespace GameEngine
{
	class TextureArray : public Texture
	{
	public:
		TextureArray ();
		virtual ~TextureArray () = 0;

		virtual void Destroy () override;

        uint32 GetArraySize () const;
		virtual bool UpdateTextureResource () override;

	protected:
        uint32 m_arraySize;
	};
}
