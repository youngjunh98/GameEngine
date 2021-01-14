#pragma once

#include "Engine/Platform/Generic/GenericFileSystem.h"
#include "Engine/Platform/PlatformMacro.h"

namespace GameEngine
{
	namespace Platform
	{
		class ENGINE_PLATFORM_API WindowsFileSystem : public GenericFileSystem
		{
		public:
			WindowsFileSystem ();
			virtual ~WindowsFileSystem ();

			virtual bool CombinePath (path_char* path, const uint32 maxPathSize, const path_char* path1, const path_char* path2) override;
			virtual bool GetParentPath (path_char* path, const uint32 maxPathSize) override;
			virtual PathString GetFileName (const path_char* path, const uint32 maxPathSize) override;
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