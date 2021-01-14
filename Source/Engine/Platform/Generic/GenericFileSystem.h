#pragma once

#include <string>
#include <vector>

#include "Type.h"
#include "Engine/Platform/PlatformMacro.h"

namespace GameEngine
{
	#if defined (ENGINE_PLATFORM_WINDOWS)
		#if defined(UNICODE) | defined(_UNICODE)
			using PathString = std::wstring;
			#define PATH(STR) L##STR
		#else
			using PathString = std::string;
			#define PATH(STR) STR
		#endif
	#endif

	namespace Platform
	{
		class ENGINE_PLATFORM_API GenericFileSystem
		{
		public:
			GenericFileSystem ();
			virtual ~GenericFileSystem () = 0;

			virtual bool CombinePath (path_char* path, const uint32 maxPathSize, const path_char* path1, const path_char* path2) = 0;
			virtual bool GetParentPath (path_char* path, const uint32 maxPathSize) = 0;
			virtual PathString GetFileName (const path_char* path, const uint32 maxPathSize) = 0;
			virtual PathString GetFileExtension (const path_char* path, const uint32 maxPathSize) = 0;
			virtual bool RemoveFileName (path_char* path, const uint32 maxPathSize) = 0;
			virtual bool AddDirectorySeparator (path_char* path, const uint32 maxPathSize) = 0;

			virtual bool FileExists (const path_char* path) = 0;
			virtual bool DirectoryExists (const path_char* path) = 0;

			virtual std::vector<PathString> GetFileList (const path_char* path) = 0;
			virtual std::vector<PathString> GetDirectoryList (const path_char* path) = 0;
		};
	}
}
