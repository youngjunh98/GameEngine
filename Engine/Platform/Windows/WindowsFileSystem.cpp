#pragma comment (lib, "Pathcch.lib")

#include <Windows.h>
#include <PathCch.h>

#include "Platform/Windows/WindowsFileSystem.h"

namespace GameEngine
{
	WindowsFileSystem::WindowsFileSystem ()
	{
	}

	WindowsFileSystem::~WindowsFileSystem ()
	{
	}

	PlatformPathType WindowsFileSystem::AppendPath (const PlatformPathType& basePath, const PlatformPathType& pathToAppend)
	{
		TCHAR combined[MAX_PATH] = { 0 };
		TCHAR append[MAX_PATH] = { 0 };

		size_t basePathLength = basePath.size ();
		size_t baseCopyMax = basePathLength <= MAX_PATH ? basePathLength : MAX_PATH;

		for(size_t i = 0; i < baseCopyMax; i++)
		{
			combined[i] = basePath[i];
		}

		size_t appendPathLength = pathToAppend.size ();
		size_t appendCopyMax = appendPathLength <= MAX_PATH ? appendPathLength : MAX_PATH;

		for(size_t i = 0; i < appendCopyMax; i++)
		{
			append[i] = pathToAppend[i];
		}

		if (SUCCEEDED (PathCchAppend(combined, MAX_PATH, append)))
		{
			return PlatformPathType (combined);
		}

		return PlatformPathType ();
	}

	bool WindowsFileSystem::FileExists (const PlatformPathType& path)
	{
		bool bResult = false;
		DWORD attributes = GetFileAttributes(path.c_str ());

		if (attributes != INVALID_FILE_ATTRIBUTES)
		{
			bResult = (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
		}

		return bResult;
	}

	bool WindowsFileSystem::DirectoryExists (const PlatformPathType& path)
	{
		bool bResult = false;
		DWORD attributes = GetFileAttributes(path.c_str ());

		if (attributes != INVALID_FILE_ATTRIBUTES)
		{
			bResult = (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
		}

		return bResult;
	}

	std::vector<PlatformPathType> WindowsFileSystem::GetFileList (const PlatformPathType& path)
	{
		std::vector<PlatformPathType> fileList;

		WIN32_FIND_DATA searchData;
		HANDLE search = FindFirstFile ((path + L"*").c_str (), &searchData);

		if (search == INVALID_HANDLE_VALUE)
		{
			return fileList;
		}

		while (FindNextFile (search, &searchData) != FALSE)
		{
			bool bDirectory = (searchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

			if (bDirectory == false && searchData.cFileName[0] != L'.')
			{
				fileList.push_back (searchData.cFileName);
			}
		}

		if (search != INVALID_HANDLE_VALUE)
		{
			FindClose (search);
		}

		return fileList;
	}

	std::vector<PlatformPathType> WindowsFileSystem::GetDirectoryList (const PlatformPathType& path)
	{
		std::vector<PlatformPathType> directoryList;

		WIN32_FIND_DATA searchData;
		HANDLE search = FindFirstFile ((path + L"*").c_str (), &searchData);

		if (search == INVALID_HANDLE_VALUE)
		{
			return directoryList;
		}

		while (FindNextFile (search, &searchData) != FALSE)
		{
			bool bDirectory = (searchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

			if (bDirectory && searchData.cFileName[0] != L'.')
			{
				directoryList.push_back (searchData.cFileName);
			}
		}

		if (search != INVALID_HANDLE_VALUE)
		{
			FindClose (search);
		}

		return directoryList;
	}
}