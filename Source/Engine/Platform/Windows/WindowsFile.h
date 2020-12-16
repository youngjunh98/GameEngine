#pragma once

#include "Engine/Platform/Generic/GenericFile.h"

namespace GameEngine
{
	namespace Platform
	{
		class ENGINE_PLATFORM_API WindowsFile : public GenericFile
		{
		public:
			WindowsFile ();
			virtual ~WindowsFile ();

			virtual bool Open (const path_char* path, EFileAccessMode fileAccessMode) override;
			virtual void Close () override;

			virtual int64 Read (void* buffer, int64 readSize) override;
			virtual int64 Write (const void* buffer, int64 writeSize) override;

			virtual int64 GetSize () const override;

			virtual bool GetPointer (int64& pointer) const override;
			virtual bool SetPointer (int64 pointer, EFilePointerMode mode) override;
		};
	}
}
