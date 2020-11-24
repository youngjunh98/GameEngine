#pragma once

#include "Platform/Generic/GenericFileSystem.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	namespace Platform
	{
		class PLATFORM_API WindowsFileSystem : public GenericFileSystem
		{
		public:
			WindowsFileSystem ();
			virtual ~WindowsFileSystem ();

			virtual bool AppendPath (path_char* path, const uint32 maxPathSize, const path_char* pathToAppend) override;
			virtual PathString GetFileExtension (const path_char* path, const uint32 maxPathSize) override;
			virtual bool RemoveFileName (path_char* path, const uint32 maxPathSize) override;
			virtual bool AddDirectorySeparator (path_char* path, const uint32 maxPathSize);

			virtual bool FileExists (const path_char* path) override;
			virtual bool DirectoryExists (const path_char* path) override;

			virtual std::vector<PathString> GetFileList (const path_char* path) override;
			virtual std::vector<PathString> GetDirectoryList (const path_char* path) override;

		protected:
			
		};
	}
}