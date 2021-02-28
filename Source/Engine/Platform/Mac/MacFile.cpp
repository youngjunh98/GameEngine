#include "MacFile.h"

namespace GameEngine
{
	namespace Platform
	{
		MacFile::MacFile ()
		{
		}

		MacFile::~MacFile ()
		{
		}

		bool MacFile::Open (const path_char* path, EFileAccessMode fileAccessMode)
		{
			return true;
		}

		void MacFile::Close ()
		{
		}

		int64 MacFile::Read (void* buffer, int64 readSize)
		{
			int64 result = 0;
			return result;
		}

		int64 MacFile::Write (const void* buffer, int64 writeSize)
		{
			int64 result = 0;
			return result;
		}
		
		int64 MacFile::GetSize () const
		{
			int64 result = 0;
			return result;
		}
		
		bool MacFile::GetPointer (int64& pointer) const
		{
			bool bSucceed = false;
			return bSucceed;
		}

		bool MacFile::SetPointer (int64 pointer, EFilePointerMode mode)
		{
            return true;
		}
	}
}
