#include "TextureArray.h"

namespace GameEngine
{
    TextureArray::TextureArray () : m_arraySize (0)
    {
    }

    TextureArray::~TextureArray ()
    {
    }

    void TextureArray::Destroy ()
    {
        Texture::Destroy ();
        m_arraySize = 0;
    }

    uint32 TextureArray::GetArraySize () const
    {
        return m_arraySize;
    }

    bool TextureArray::UpdateTextureResource ()
    {
        bool bSucceed = false;

        if (Texture::UpdateTextureResource ())
        {
            m_arraySize = m_data.m_array.size ();
            bSucceed = true;
        }

        return bSucceed;
    }
}
