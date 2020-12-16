#include <Windows.h>

#include "Engine/Platform/Windows/WindowsFile.h"

namespace GameEngine
{
	namespace Platform
	{
		WindowsFile::WindowsFile ()
		{
		}

		WindowsFile::~WindowsFile ()
		{
			HANDLE handle = static_cast<HANDLE> (m_fileHandle);

			if (handle != NULL && handle != INVALID_HANDLE_VALUE)
			{
				CloseHandle (handle);
			}
		}

		bool WindowsFile::Open (const path_char* path, EFileAccessMode fileAccessMode)
		{
			if (IsOpen ())
			{
				return false;
			}

			DWORD accessMode = 0;
			DWORD shareMode = 0;
			DWORD creationDisposition = OPEN_EXISTING;

			if (fileAccessMode == EFileAccessMode::Read || fileAccessMode == EFileAccessMode::ReadWrite)
			{
				accessMode |= GENERIC_READ;
				shareMode |= FILE_SHARE_READ;
			}

			if (fileAccessMode == EFileAccessMode::Write || fileAccessMode == EFileAccessMode::ReadWrite)
			{
				accessMode |= GENERIC_WRITE;
				shareMode |= FILE_SHARE_WRITE;
				creationDisposition = CREATE_ALWAYS;
			}

			DWORD flags = 0;
			DWORD attributes = FILE_ATTRIBUTE_NORMAL;

			HANDLE handle = CreateFile (path, accessMode, shareMode, nullptr, creationDisposition, flags | attributes, nullptr);

			if (handle == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			m_fileHandle = handle;
			m_bOpen = true;
			m_accessMode = fileAccessMode;

			return true;
		}

		void WindowsFile::Close ()
		{
			if (IsOpen ())
			{
				HANDLE handle = static_cast<HANDLE> (m_fileHandle);

				if (CloseHandle (handle) == TRUE)
				{
					m_fileHandle = nullptr;
					m_bOpen = false;
				}
			}
		}

		int64 WindowsFile::Read (void* buffer, int64 readSize)
		{
			int64 result = 0;

			if (IsRead ())
			{
				HANDLE handle = static_cast<HANDLE> (m_fileHandle);
				DWORD bytesToRead = static_cast<DWORD> (readSize);
				DWORD bytesRead;

				if (ReadFile (handle, buffer, bytesToRead, &bytesRead, nullptr) == TRUE)
				{
					result = static_cast<int64> (bytesRead);
				}
			}

			return result;
		}

		int64 WindowsFile::Write (const void* buffer, int64 writeSize)
		{
			int64 result = 0;

			if (IsWrite ())
			{
				HANDLE handle = static_cast<HANDLE> (m_fileHandle);
				DWORD bytesToWrite = static_cast<DWORD> (writeSize);
				DWORD bytesWritten;

				if (WriteFile (handle, buffer, bytesToWrite, &bytesWritten, nullptr) == TRUE)
				{
					result = static_cast<int64> (bytesWritten);
				}
			}

			return result;
		}
		
		int64 WindowsFile::GetSize () const
		{
			int64 result = 0;

			if (IsOpen ())
			{
				HANDLE handle = static_cast<HANDLE> (m_fileHandle);
				LARGE_INTEGER size;

				if (GetFileSizeEx (handle, &size) == TRUE)
				{
					result = size.QuadPart;
				}
			}

			return result;
		}
		
		bool WindowsFile::GetPointer (int64& pointer) const
		{
			if (IsOpen () == false)
			{
				return false;
			}

			bool bSucceed = false;
			HANDLE handle = static_cast<HANDLE> (m_fileHandle);

			LARGE_INTEGER zero;
			zero.QuadPart = 0;

			LARGE_INTEGER current;

			if (SetFilePointerEx (handle, zero, &current, FILE_CURRENT) == TRUE)
			{
				pointer = current.QuadPart;
				bSucceed = true;
			}

			return bSucceed;
		}

		bool WindowsFile::SetPointer (int64 pointer, EFilePointerMode mode)
		{
			if (IsOpen () == false)
			{
				return false;
			}

			LARGE_INTEGER move;
			move.QuadPart = pointer;

			DWORD moveMethod = FILE_BEGIN;

			if (mode == EFilePointerMode::Current)
			{
				moveMethod = FILE_CURRENT;
			}
			else if (mode == EFilePointerMode::End)
			{
				moveMethod = FILE_END;
			}

			HANDLE handle = static_cast<HANDLE> (m_fileHandle);

			return SetFilePointerEx (handle, move, nullptr, moveMethod) == TRUE;
		}
	}
}
