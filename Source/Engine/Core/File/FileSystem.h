#pragma once

#include <memory>

#include "Type.h"
#include "Engine/Platform/Platform.h"
#include "Engine/Core/CoreMacro.h"

namespace GameEngine
{
    class ENGINE_CORE_API FileSystem
    {
    public:
        FileSystem ();
        virtual ~FileSystem () = 0;

        static PathString CombinePath (const PathString& path, const PathString& pathToAppend);
        static PathString GetParentDirectory (const PathString& path);
        static PathString GetFileName (const PathString& path);
        static PathString GetFileExtension (const PathString& path);
        static PathString SetFileExtension (const PathString& path, const PathString& extension);
        static PathString RemoveFileName (const PathString& path);
        static PathString AddDirectorySeparator (const PathString& path);

		static bool FileExists (const PathString& path);
		static bool DirectoryExists (const PathString& path);

		static std::vector<PathString> GetFileNames (const PathString& path);
		static std::vector<PathString> GetDirectoryNames (const PathString& path);

    private:
        static bool CreatePathBuffer (std::unique_ptr<path_char[]>& buffer, uint32 bufferSize, const PathString& initPath);
    };
}