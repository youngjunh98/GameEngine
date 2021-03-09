#include "AssetData.h"

namespace GameEngine
{
    TextureData::MipMapData::MipMapData () : m_width (0), m_height (0)
    {
    }

    TextureData::MipMapData::~MipMapData ()
    {
        m_pixels.clear ();
        m_width = 0;
        m_height = 0;
    }

    TextureData::TextureData () : m_channels (0), m_bytes (0), m_bLinear (false),
        m_format (ERenderingResourceFormat::None)
    {
    }

    TextureData::~TextureData ()
    {
        m_channels = 0;
        m_bytes = 0;
        m_bLinear = false;
        m_format = ERenderingResourceFormat::None;
    }

    uint32 TextureData::GetDataSizeInBytes (uint32 arrayIndex, uint32 mipMapIndex) const
    {
        uint32 result = 0;

        if (arrayIndex >= 0 && arrayIndex < m_array.size ())
        {
            if (mipMapIndex >= 0 && mipMapIndex < m_array.at (arrayIndex).m_mipMaps.size ())
            {
                const MipMapData& mipMap = m_array.at (arrayIndex).m_mipMaps.at (mipMapIndex);
                result = mipMap.m_width * mipMap.m_height * m_channels * m_bytes;
            }
        }
        
        return result;
    }

    uint32 TextureData::GetRowSizeInBytes (uint32 arrayIndex, uint32 mipMapIndex) const
    {
        uint32 result = 0;

        if (arrayIndex >= 0 && arrayIndex < m_array.size ())
        {
            if (mipMapIndex >= 0 && mipMapIndex < m_array.at (arrayIndex).m_mipMaps.size ())
            {
                const MipMapData& mipMap = m_array.at (arrayIndex).m_mipMaps.at (mipMapIndex);
                result = mipMap.m_width * m_channels * m_bytes;
            }
        }
        
        return result;
    }

    bool TextureData::IsSquare () const
    {
        bool bResult = false;

        if (m_array.size () > 0)
        {
            if (m_array.at (0).m_mipMaps.size () > 0)
            {
                const MipMapData& mipMap = m_array.at (0).m_mipMaps.at (0);
                bResult = (mipMap.m_width == mipMap.m_height) && (mipMap.m_width > 0) && (mipMap.m_height > 0);
            }
        }

        return bResult;
    }

    bool TextureData::IsPowerOfTwo () const
    {
        bool bResult = false;

        if (m_array.size () > 0)
        {
            if (m_array.at (0).m_mipMaps.size () > 0)
            {
                const MipMapData& mipMap = m_array.at (0).m_mipMaps.at (0);
                bResult = Math::IsPowerOfTwo (mipMap.m_width) && Math::IsPowerOfTwo (mipMap.m_height);
            }
        }

        return bResult;
    }

    bool TextureData::AddTextureArray (const TextureData& texture)
    {
        bool bSucceed = false;

        if (m_array.size () > 0 && texture.m_array.size () > 0)
        {
            const MipMapData& mipMap = m_array.at (0).m_mipMaps.at (0);
            const MipMapData& otherMipMap = texture.m_array.at (0).m_mipMaps.at (0);

            bool bSameSize = (mipMap.m_width == otherMipMap.m_width) && (mipMap.m_height == otherMipMap.m_height);
            bool bSameMipMapLevels = m_array.at (0).m_mipMaps.size () == texture.m_array.at (0).m_mipMaps.size ();
            bool bSameFormat = m_channels == texture.m_channels && m_bytes == texture.m_bytes && m_bLinear == texture.m_bLinear && m_format == texture.m_format;

            if (bSameSize && bSameMipMapLevels && bSameFormat)
            {
                bool bSameMipMapSizes = true;
                auto it = m_array.at (0).m_mipMaps.begin ();
                auto otherIt = texture.m_array.at (0).m_mipMaps.begin ();

                while (it != m_array.at (0).m_mipMaps.end ())
                {
                    if (it->m_width != otherIt->m_width || it->m_height != otherIt->m_height)
                    {
                        bSameMipMapSizes = false;
                        break;
                    }

                    ++it;
                    ++otherIt;
                }

                if (bSameMipMapSizes)
                {
                    m_array.insert (m_array.end (), texture.m_array.begin (), texture.m_array.end ());
                    bSucceed = true;
                }
            }
        }

        return bSucceed;
    }
}
