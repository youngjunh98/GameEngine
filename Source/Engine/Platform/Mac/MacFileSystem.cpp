#include "MacFileSystem.h"

namespace GameEngine
{
	namespace Platform
	{
		MacFileSystem::MacFileSystem ()
		{
		}

		MacFileSystem::~MacFileSystem ()
		{
		}

		bool MacFileSystem::CombinePath (path_char* path, const uint32 maxPathSize, const path_char* path1, const path_char* path2)
		{
			return false;
		}

		bool MacFileSystem::GetParentPath (path_char* path, const uint32 maxPathSize)
		{
			return false;
		}

		PathString MacFileSystem::GetFileName (const path_char* path, const uint32 maxPathSize)
		{
			return PathString ();
		}

		PathString MacFileSystem::GetFileExtension (const path_char* path, const uint32 maxPathSize)
		{
			PathString extension = PATH ("");
			return extension;
		}

		bool MacFileSystem::SetFileExtension (path_char* path, const uint32 maxPathSize, const path_char* extension)
		{
			bool bSucceed = false;
			return bSucceed;
		}

		bool MacFileSystem::RemoveFileName (path_char* path, const uint32 maxPathSize)
		{
			bool bSucceed = false;
			return bSucceed;
		}

		bool MacFileSystem::AddDirectorySeparator (path_char* path, const uint32 maxPathSize)
		{
			return false;
		}

		bool MacFileSystem::FileExists (const path_char* path)
		{
			bool bResult = false;
			return bResult;
		}

		bool MacFileSystem::DirectoryExists (const path_char* path)
		{
			bool bResult = false;
			return bResult;
		}

		std::vector<PathString> MacFileSystem::GetFileNames (const path_char* path)
		{
			std::vector<PathString> foundList;
			return foundList;
		}

		std::vector<PathString> MacFileSystem::GetDirectoryNames (const path_char* path)
		{
			std::vector<PathString> foundList;
			return foundList;
		}
	}
}
