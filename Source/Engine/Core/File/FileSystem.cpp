#include <memory>
#include <cstring>

#include "Engine/Core/File/FileSystem.h"

namespace GameEngine
{
    static Platform::FileSystem g_platformFileSystem;

    FileSystem::FileSystem () {}
    FileSystem::~FileSystem () {}

    PathString FileSystem::CombinePath (const PathString& path, const PathString& pathToAppend)
    {
        PathString result = path;
        PathString separatorAdded = AddDirectorySeparator (path);

        const path_char* originalPathStart = separatorAdded.c_str ();
        size_t originalPathSize = separatorAdded.size ();

        const path_char* appendPathStart = pathToAppend.c_str ();
        size_t appendPathSize = pathToAppend.size ();

        size_t bufferSize = originalPathSize + appendPathSize + 1;
        auto buffer = std::make_unique<path_char[]> (bufferSize);
        path_char* bufferStart = buffer.get ();

        std::memset (bufferStart, PATH ('\0'), bufferSize * sizeof (path_char));

        if (g_platformFileSystem.CombinePath (bufferStart, bufferSize, originalPathStart, appendPathStart) == true)
        {
            result = PathString (bufferStart);
        }
        
        return result;
    }

    PathString FileSystem::GetFileName (const PathString& path)
    {
        return g_platformFileSystem.GetFileName (path.c_str (), path.size () + 1);
    }

    PathString FileSystem::GetFileExtension (const PathString& path)
    {
        const path_char* pathStart = path.c_str ();
        uint32 pathSize = path.size () + 1;

        return g_platformFileSystem.GetFileExtension (pathStart, pathSize);
    }

    PathString FileSystem::RemoveFileName (const PathString& path)
    {
        PathString result = path;

        const path_char* originalPathStart = path.c_str ();
        size_t originalPathSize = path.size ();

        size_t bufferSize = originalPathSize + 1;
        auto buffer = std::make_unique<path_char[]> (bufferSize);
        path_char* bufferStart = buffer.get ();

        std::memset (bufferStart, PATH ('\0'), bufferSize * sizeof (path_char));
        std::memcpy (bufferStart, originalPathStart, originalPathSize * sizeof (path_char));

        if (g_platformFileSystem.RemoveFileName (bufferStart, bufferSize) == true)
        {
            result = PathString (bufferStart);
        }

        return result;
    }

    PathString FileSystem::AddDirectorySeparator (const PathString& path)
    {
        PathString result = path;

        const path_char* originalPathStart = path.c_str ();
        size_t originalPathSize = path.size ();

        // original path + null character + separator + extra
        size_t bufferSize = originalPathSize + 10;
        auto buffer = std::make_unique<path_char[]> (bufferSize);
        path_char* bufferStart = buffer.get ();

        std::memset (bufferStart, PATH ('\0'), bufferSize * sizeof (path_char));
        std::memcpy (bufferStart, originalPathStart, originalPathSize * sizeof (path_char));

        if (g_platformFileSystem.AddDirectorySeparator (bufferStart, bufferSize) == true)
        {
            result = PathString (bufferStart);
        }

        return result;
    }

    bool FileSystem::FileExists (const PathString& path)
    {
        return g_platformFileSystem.FileExists (path.c_str ());
    }

    bool FileSystem::DirectoryExists (const PathString& path)
    {
        return g_platformFileSystem.DirectoryExists (path.c_str ());
    }

    std::vector<PathString> FileSystem::GetFileList (const PathString& path)
    {
        return g_platformFileSystem.GetFileList (AddDirectorySeparator (path).c_str ());
    }

    std::vector<PathString> FileSystem::GetDirectoryList (const PathString& path)
    {
        return g_platformFileSystem.GetDirectoryList (AddDirectorySeparator (path).c_str ());
    }
}