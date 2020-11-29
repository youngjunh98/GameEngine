#include "Core/File/File.h"

namespace GameEngine
{
    File::File (const PathString& path, EFileAccessMode accessMode) :
        m_bOpen ()
    {
        m_bOpen = m_platformFile.Open (path.c_str (), accessMode);
    }

    File::~File ()
    {
        if (m_bOpen)
        {
            m_platformFile.Close ();
            m_bOpen = false;
        }
    }

    int64 File::Read (void* buffer, int64 readSize)
    {
        return m_platformFile.Read (buffer, readSize);
    }

    int64 File::ReadAll (void* buffer)
    {
        int64 bytesRead = 0;

        if (m_platformFile.SetPointer (0, EFilePointerMode::Begin))
        {
            int64 fileSize = GetSize ();
            bytesRead = m_platformFile.Read (buffer, fileSize);
        }

        return bytesRead;
    }

    int64 File::Write (const void* buffer, int64 writeSize)
    {
        return m_platformFile.Write (buffer, writeSize);
    }

    int64 File::GetSize () const
    {
        return m_platformFile.GetSize ();
    }

    bool File::IsOpen () const
    {
        return m_bOpen;
    }
}