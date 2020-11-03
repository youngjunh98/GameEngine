#pragma once

#include <string>
#include <vector>

#include "Core/CoreType.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	class PLATFORM_API GenericPlatformFileSystem
	{
	public:
		GenericPlatformFileSystem ();
		virtual ~GenericPlatformFileSystem () = 0;

		virtual std::vector<std::wstring> GetDirectoryList (const std::wstring& path) = 0;
		virtual std::vector<std::wstring> GetFileList (const std::wstring& path) = 0;
	};
}
