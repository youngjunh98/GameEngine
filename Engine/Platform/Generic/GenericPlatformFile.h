#pragma once

#include <string>

#include "Platform/PlatformMacro.h"
#include "Core/CoreType.h"
#include "Core/File/FileEnum.h"

#if defined(PLATFORM_WINDOWS)

#endif

namespace GameEngine
{
	#if defined(UNICODE) | defined(_UNICODE)
		using PlatformPathType = std::wstring;
	#else
		using PlatformPathType = std::string;
	#endif

	class PLATFORM_API GenericPlatformFile
	{
	public:
		GenericPlatformFile ();
		virtual ~GenericPlatformFile () = 0;

		virtual bool Open (const PlatformPathType& path, bool bRead, bool bWrite) = 0;
		virtual void Close () = 0;

		virtual int64 Read (void* buffer, int64 readSize) = 0;
		virtual int64 Write (const void* buffer, int64 writeSize) = 0;

		virtual int64 GetSize () const = 0;

		virtual bool GetPointer (int64& pointer) const = 0;
		virtual bool SetPointer (int64 pointer, EFilePointerMode mode) = 0;

		bool IsOpen () const;
		bool IsRead () const;
		bool IsWrite () const;

	protected:
		bool m_bOpen;
		bool m_bRead;
		bool m_bWrite;
	};
}
