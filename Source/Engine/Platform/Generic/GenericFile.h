#pragma once

#include "Type.h"
#include "Engine/Platform/PlatformMacro.h"
#include "Engine/Core/File/FileEnum.h"

namespace GameEngine
{
	namespace Platform
	{
		class ENGINE_PLATFORM_API GenericFile
		{
		public:
			GenericFile ();
			virtual ~GenericFile () = 0;

			virtual bool Open (const path_char* path, EFileAccessMode fileAccessMode) = 0;
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
			void* m_fileHandle;
			bool m_bOpen;
			EFileAccessMode m_accessMode;
		};
	}
}
