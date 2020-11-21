#pragma once

#include <string>
#include <vector>

#include "EngineType.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	#if defined (PLATFORM_WINDOWS)
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
		class PLATFORM_API GenericFileSystem
		{
		public:
			GenericFileSystem ();
			virtual ~GenericFileSystem () = 0;

			virtual bool AppendPath (path_char* path, const uint32 maxPathSize, const path_char* pathToAppend) = 0;
			virtual PathString GetFileExtension (const path_char* path, const uint32 maxPathSize) = 0;

			virtual bool FileExists (const path_char* path) = 0;
			virtual bool DirectoryExists (const path_char* path) = 0;

			virtual std::vector<PathString> GetFileList (const path_char* path) = 0;
			virtual std::vector<PathString> GetDirectoryList (const path_char* path) = 0;
		};
	}
}
