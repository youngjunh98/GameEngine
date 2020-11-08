#pragma once

#include "Platform/Generic/GenericPlatformFileSystem.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	class PLATFORM_API WindowsFileSystem : public GenericPlatformFileSystem
	{
	public:
		WindowsFileSystem ();
		virtual ~WindowsFileSystem ();

		virtual PlatformPathType AppendPath (const PlatformPathType& basePath, const PlatformPathType& pathToAppend) override;
		
		virtual bool FileExists (const PlatformPathType& path) override;
		virtual bool DirectoryExists (const PlatformPathType& path) override;

		virtual std::vector<PlatformPathType> GetFileList (const PlatformPathType& path) override;
		virtual std::vector<PlatformPathType> GetDirectoryList (const PlatformPathType& path) override;
	};
}