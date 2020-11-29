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

        static PathString AppendPath (const PathString& path, const PathString& pathToAppend);
        static PathString GetFileExtension (const PathString& path);
        static PathString RemoveFileName (const PathString& path);
        static PathString AddDirectorySeparator (const PathString& path);

		static bool FileExists (const PathString& path);
		static bool DirectoryExists (const PathString& path);

		static std::vector<PathString> GetFileList (const PathString& path);
		static std::vector<PathString> GetDirectoryList (const PathString& path);
    };
}