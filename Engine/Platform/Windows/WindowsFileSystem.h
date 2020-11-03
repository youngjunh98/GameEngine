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

		virtual std::vector<std::wstring> GetDirectoryList (const std::wstring& path) override;
		virtual std::vector<std::wstring> GetFileList (const std::wstring& path) override;
	};
}