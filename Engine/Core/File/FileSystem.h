#pragma once

#include "Platform/Platform.h"
#include "Core/CoreMacro.h"

namespace GameEngine
{
    class CORE_API FileSystem
    {
    public:
        FileSystem ();
        virtual ~FileSystem () = 0;

        static PlatformPathType AppendPath (const PlatformPathType& basePath, const PlatformPathType& pathToAppend);

		static bool FileExists (const PlatformPathType& path);
		static bool DirectoryExists (const PlatformPathType& path);

		static std::vector<PlatformPathType> GetFileList (const PlatformPathType& path);
		static std::vector<PlatformPathType> GetDirectoryList (const PlatformPathType& path);
    };
}