#pragma comment (lib, "Shlwapi.lib")
#pragma comment (lib, "Pathcch.lib")

#include <Windows.h>
#include <Shlwapi.h>
#include <PathCch.h>

#include "Engine/Platform/Windows/WindowsFileSystem.h"

namespace GameEngine
{
	namespace Platform
	{
		WindowsFileSystem::WindowsFileSystem ()
		{
		}

		WindowsFileSystem::~WindowsFileSystem ()
		{
		}

		bool WindowsFileSystem::CombinePath (path_char* path, const uint32 maxPathSize, const path_char* path1, const path_char* path2)
		{
			return SUCCEEDED (PathCchCombine(path, maxPathSize, path1, path2));
		}

		bool WindowsFileSystem::GetParentPath (path_char* path, const uint32 maxPathSize)
		{
			RemoveFileName (path, maxPathSize);
			return SUCCEEDED (PathCchRemoveFileSpec (path, maxPathSize));
		}

		PathString WindowsFileSystem::GetFileName (const path_char* path, const uint32 maxPathSize)
		{
			return PathString (PathFindFileName (path));
		}

		PathString WindowsFileSystem::GetFileExtension (const path_char* path, const uint32 maxPathSize)
		{
			PathString extension = PATH("");
			PCTSTR dotBeforeExtension;

			if (SUCCEEDED (PathCchFindExtension (path, maxPathSize, &dotBeforeExtension)))
			{
				extension = PathString (dotBeforeExtension + 1);
			}

			return extension;
		}

		bool WindowsFileSystem::SetFileExtension (path_char* path, const uint32 maxPathSize, const path_char* extension)
		{
			return SUCCEEDED (PathCchRenameExtension (path, maxPathSize, extension));
		}

		bool WindowsFileSystem::RemoveFileName (path_char* path, const uint32 maxPathSize)
		{
			bool bSucceed = false;

			if (FileExists (path))
			{
				bSucceed = SUCCEEDED (PathCchRemoveFileSpec (path, maxPathSize));
			}

			return bSucceed;
		}

		bool WindowsFileSystem::AddDirectorySeparator (path_char* path, const uint32 maxPathSize)
		{
			return SUCCEEDED (PathCchAddBackslash (path, maxPathSize));
		}

		bool WindowsFileSystem::FileExists (const path_char* path)
		{
			bool bResult = false;
			DWORD attributes = GetFileAttributes(path);

			if (attributes != INVALID_FILE_ATTRIBUTES)
			{
				bResult = (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
			}

			return bResult;
		}

		bool WindowsFileSystem::DirectoryExists (const path_char* path)
		{
			bool bResult = false;
			DWORD attributes = GetFileAttributes(path);

			if (attributes != INVALID_FILE_ATTRIBUTES)
			{
				bResult = (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			}

			return bResult;
		}

		std::vector<PathString> WindowsFileSystem::GetFileNames (const path_char* path)
		{
			std::vector<PathString> foundList;

			PathString searchPattern (path);
			searchPattern += PATH ("*");

			WIN32_FIND_DATA searchData;
			HANDLE search = FindFirstFile (searchPattern.c_str (), &searchData);

			if (search != INVALID_HANDLE_VALUE)
			{
				while (FindNextFile (search, &searchData) != FALSE)
				{
					bool bDirectory = (searchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

					if (bDirectory == false && searchData.cFileName[0] != PATH('.'))
					{
						foundList.push_back (searchData.cFileName);
					}
				}

				FindClose (search);
			}

			return foundList;
		}

		std::vector<PathString> WindowsFileSystem::GetDirectoryNames (const path_char* path)
		{
			std::vector<PathString> foundList;

			PathString searchPattern (path);
			searchPattern += PATH ("*");

			WIN32_FIND_DATA searchData;
			HANDLE search = FindFirstFile (searchPattern.c_str (), &searchData);

			if (search != INVALID_HANDLE_VALUE)
			{
				while (FindNextFile (search, &searchData) != FALSE)
				{
					bool bDirectory = (searchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

					if (bDirectory && searchData.cFileName[0] != PATH('.'))
					{
						foundList.push_back (searchData.cFileName);
					}
				}

				FindClose (search);
			}

			return foundList;
		}
	}
}