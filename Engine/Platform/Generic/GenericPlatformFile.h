#pragma once

#include <string>
#include <vector>

#include "Core/CoreType.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	class PLATFORM_API GenericPlatformFile
	{
	public:
		GenericPlatformFile ();
		virtual ~GenericPlatformFile () = 0;

		virtual bool Open (const std::wstring& path, bool bRead, bool bWrite) = 0;
		virtual void Close () = 0;

		virtual void Read (void* buffer, int64 startOffset, int64 readSize) = 0;
		virtual void ReadAll (void* buffer, int64 bufferSize) = 0;

		virtual void Write (const void* buffer, int64 bufferSize) = 0;

		bool IsOpen () const;
		virtual int64 GetSize () const = 0;

	protected:
		bool m_bOpen;
		bool m_bRead;
		bool m_bWrite;
	};
}
