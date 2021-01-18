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

        size_t bufferSize = separatorAdded.size () + pathToAppend.size () + 1;
        std::unique_ptr<path_char[]> buffer;

        if (CreatePathBuffer (buffer, bufferSize, PATH("")))
        {
            path_char* bufferStart = buffer.get ();
            const path_char* originalPathStart = separatorAdded.c_str ();
            const path_char* appendPathStart = pathToAppend.c_str ();

            if (g_platformFileSystem.CombinePath (bufferStart, bufferSize, originalPathStart, appendPathStart) == true)
            {
                result = PathString (bufferStart);
            }
        }

        return result;
    }

    PathString FileSystem::GetParentDirectory (const PathString& path)
    {
        PathString result = path;

        size_t bufferSize = path.size () + 2;
        std::unique_ptr<path_char[]> buffer;

        if (CreatePathBuffer (buffer, bufferSize, path))
        {
            path_char* bufferStart = buffer.get ();

            if (g_platformFileSystem.GetParentPath (bufferStart, bufferSize))
            {
                result = PathString (bufferStart);
            }
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

    PathString FileSystem::SetFileExtension (const PathString& path, const PathString& extension)
    {
        PathString result = path;

        // original + dot + extension + null
        size_t bufferSize = path.size () + extension.size () + 2;
        std::unique_ptr<path_char[]> buffer;

        if (CreatePathBuffer (buffer, bufferSize, path))
        {
            path_char* bufferStart = buffer.get ();
            const path_char* extensionStart = extension.c_str ();

            if (g_platformFileSystem.SetFileExtension (bufferStart, bufferSize, extensionStart))
            {
                result = PathString (bufferStart);
            }
        }

        return result;
    }

    PathString FileSystem::RemoveFileName (const PathString& path)
    {
        PathString result = path;

        size_t bufferSize = path.size ()+ 1;
        std::unique_ptr<path_char[]> buffer;

        if (CreatePathBuffer (buffer, bufferSize, path))
        {
            path_char* bufferStart = buffer.get ();

            if (g_platformFileSystem.RemoveFileName (bufferStart, bufferSize) == true)
            {
                result = PathString (bufferStart);
            }
        }

        return result;
    }

    PathString FileSystem::AddDirectorySeparator (const PathString& path)
    {
        PathString result = path;

        // original path + null character + separator + extra
        size_t bufferSize = path.size () + 10;
        std::unique_ptr<path_char[]> buffer;

        if (CreatePathBuffer (buffer, bufferSize, path))
        {
            path_char* bufferStart = buffer.get ();

            if (g_platformFileSystem.AddDirectorySeparator (bufferStart, bufferSize) == true)
            {
                result = PathString (bufferStart);
            }
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

    std::vector<PathString> FileSystem::GetFileNames (const PathString& path)
    {
        return g_platformFileSystem.GetFileNames (AddDirectorySeparator (path).c_str ());
    }

    std::vector<PathString> FileSystem::GetDirectoryNames (const PathString& path)
    {
        return g_platformFileSystem.GetDirectoryNames (AddDirectorySeparator (path).c_str ());
    }

    bool FileSystem::CreatePathBuffer (std::unique_ptr<path_char[]>& buffer, uint32 bufferSize, const PathString& initPath)
    {
        bool bSucceed = false;
        buffer = std::make_unique<path_char[]> (bufferSize);

        if (buffer != nullptr)
        {
            bSucceed = true;

            // Fill buffer with null character
            path_char* bufferStart = buffer.get ();
            std::memset (bufferStart, PATH ('\0'), bufferSize * sizeof (path_char));

            // Copy initPath to buffer
            const path_char* initPathStart = initPath.c_str ();
            size_t initPathSize = initPath.size ();
            std::memcpy (bufferStart, initPathStart, initPathSize * sizeof (path_char));
        }

        return bSucceed;
    }
}