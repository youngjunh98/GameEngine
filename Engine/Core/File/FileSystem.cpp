#include <memory>
#include <cstring>

#include "Core/File/FileSystem.h"

namespace GameEngine
{
    static Platform::FileSystem g_platformFileSystem;

    FileSystem::FileSystem () {}
    FileSystem::~FileSystem () {}

    PathString FileSystem::AppendPath (const PathString& path, const PathString& pathToAppend)
    {
        PathString result = path;

        const path_char* originalPathStart = path.c_str ();
        size_t originalPathSize = path.size ();

        const path_char* appendPathStart = pathToAppend.c_str ();
        size_t appendPathSize = pathToAppend.size ();

        size_t bufferSize = originalPathSize + appendPathSize + 1;
        auto buffer = std::make_unique<path_char[]> (bufferSize);
        path_char* bufferStart = buffer.get ();

        std::memset (bufferStart, PATH ('\0'), bufferSize);
        std::memcpy (bufferStart, originalPathStart, originalPathSize);

        if (g_platformFileSystem.AppendPath (bufferStart, bufferSize, appendPathStart) == true)
        {
            result = PathString (bufferStart);
        }
        
        return result;
    }

    PathString FileSystem::GetFileExtension (const PathString& path)
    {
        const path_char* pathStart = path.c_str ();
        uint32 pathSize = path.size () + 1;

        return g_platformFileSystem.GetFileExtension (pathStart, pathSize);
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
        return g_platformFileSystem.GetFileList (path.c_str ());
    }

    std::vector<PathString> FileSystem::GetDirectoryList (const PathString& path)
    {
        return g_platformFileSystem.GetDirectoryList (path.c_str ());
    }
}