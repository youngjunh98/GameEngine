#pragma once

#include <string>
#include <vector>

#include "Platform/PlatformMacro.h"
#include "Platform/Generic/GenericPlatformFile.h"
#include "Core/CoreType.h"

namespace GameEngine
{
	class PLATFORM_API GenericPlatformFileSystem
	{
	public:
		GenericPlatformFileSystem ();
		virtual ~GenericPlatformFileSystem () = 0;

		virtual PlatformPathType AppendPath (const PlatformPathType& basePath, const PlatformPathType& pathToAppend) = 0;

		virtual bool FileExists (const PlatformPathType& path) = 0;
		virtual bool DirectoryExists (const PlatformPathType& path) = 0;

		virtual std::vector<PlatformPathType> GetFileList (const PlatformPathType& path) = 0;
		virtual std::vector<PlatformPathType> GetDirectoryList (const PlatformPathType& path) = 0;
	};
}
