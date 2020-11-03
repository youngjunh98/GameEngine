#include <Windows.h>

#include "Platform/Windows/WindowsFileSystem.h"

namespace GameEngine
{
	WindowsFileSystem::WindowsFileSystem ()
	{
	}

	WindowsFileSystem::~WindowsFileSystem ()
	{
	}

	std::vector<std::wstring> WindowsFileSystem::GetDirectoryList (const std::wstring& path)
	{
		std::vector<std::wstring> directoryList;

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

	std::vector<std::wstring> WindowsFileSystem::GetFileList (const std::wstring& path)
	{
		std::vector<std::wstring> fileList;

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
}