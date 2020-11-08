#include "Core/File/FileSystem.h"

namespace GameEngine
{
    static PlatformFileSystem g_platformFileSystem;

    FileSystem::FileSystem () {}
    FileSystem::~FileSystem () {}

    PlatformPathType FileSystem::AppendPath (const PlatformPathType& basePath, const PlatformPathType& pathToAppend)
    {
        return g_platformFileSystem.AppendPath (basePath, pathToAppend);
    }

    bool FileSystem::FileExists (const PlatformPathType& path)
    {
        return g_platformFileSystem.FileExists (path);
    }

    bool FileSystem::DirectoryExists (const PlatformPathType& path)
    {
        return g_platformFileSystem.DirectoryExists (path);
    }

    std::vector<PlatformPathType> FileSystem::GetFileList (const PlatformPathType& path)
    {
        return g_platformFileSystem.GetFileList (path);
    }

    std::vector<PlatformPathType> FileSystem::GetDirectoryList (const PlatformPathType& path)
    {
        return g_platformFileSystem.GetDirectoryList (path);
    }
}