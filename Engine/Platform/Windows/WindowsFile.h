#pragma once

#include <Windows.h>

#include "Platform/Generic/GenericPlatformFile.h"

namespace GameEngine
{
	class PLATFORM_API WindowsFile : public GenericPlatformFile
	{
	public:
		WindowsFile ();
		virtual ~WindowsFile ();

		virtual bool Open (const std::wstring& path, bool bRead, bool bWrite) override;
		virtual void Close () override;

		virtual void Read (void* buffer, int64 startOffset, int64 readSize) override;
		virtual void ReadAll (void* buffer, int64 bufferSize) override;

		virtual void Write (const void* buffer, int64 bufferSize) override;

		virtual int64 GetSize () const override;

	protected:
		HANDLE m_file;
	};
}
