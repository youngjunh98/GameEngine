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

		virtual bool Open (const PlatformPathType& path, bool bRead, bool bWrite) override;
		virtual void Close () override;

		virtual int64 Read (void* buffer, int64 readSize) override;
		virtual int64 Write (const void* buffer, int64 writeSize) override;

		virtual int64 GetSize () const override;

		virtual bool GetPointer (int64& pointer) const override;
		virtual bool SetPointer (int64 pointer, EFilePointerMode mode) override;

	protected:
		HANDLE m_fileHandle;
	};
}
