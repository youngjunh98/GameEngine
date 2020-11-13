#pragma once

#include "Platform/Platform.h"
#include "Core/CoreMacro.h"
#include "Core/File/FileEnum.h"

namespace GameEngine
{
    class CORE_API File
    {
    public:
        File (const PathString& path, EFileAccessMode accessMode);
        virtual ~File ();

        int64 Read (void* buffer, int64 readSize);
        int64 ReadAll (void* buffer);
		int64 Write (const void* buffer, int64 writeSize);

		int64 GetSize () const;
        bool IsOpen () const;

    private:
        bool m_bOpen;
        Platform::File m_platformFile;
    };
}