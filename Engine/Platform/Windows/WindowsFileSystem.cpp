#pragma comment (lib, "Pathcch.lib")

#include <Windows.h>
#include <PathCch.h>

#include "Platform/Windows/WindowsFileSystem.h"

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

		bool WindowsFileSystem::AppendPath (path_char* path, const uint32 maxPathSize, const path_char* pathToAppend)
		{
			bool bSucceed = SUCCEEDED (PathCchAppend(path, maxPathSize, pathToAppend));

			return bSucceed;
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

		std::vector<PathString> WindowsFileSystem::GetFileList (const path_char* path)
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

		std::vector<PathString> WindowsFileSystem::GetDirectoryList (const path_char* path)
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