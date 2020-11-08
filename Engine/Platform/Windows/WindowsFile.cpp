#include "Platform/Windows/WindowsFile.h"

namespace GameEngine
{
	WindowsFile::WindowsFile () : m_fileHandle (NULL)
	{
	}

	WindowsFile::~WindowsFile ()
	{
		if (m_fileHandle != NULL && m_fileHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle (m_fileHandle);
		}
	}

	bool WindowsFile::Open (const PlatformPathType& path, bool bRead, bool bWrite)
	{
		if (IsOpen () || (bRead == false && bWrite == false))
		{
			return false;
		}

		DWORD accessMode = 0;
		DWORD shareMode = 0;
		DWORD creationDisposition = OPEN_EXISTING;

		if (bRead)
		{
			accessMode |= GENERIC_READ;
			shareMode |= FILE_SHARE_READ;
		}

		if (bWrite)
		{
			accessMode |= GENERIC_WRITE;
			shareMode |= FILE_SHARE_WRITE;
			creationDisposition = CREATE_ALWAYS;
		}

		DWORD flags = 0;
		DWORD attributes = FILE_ATTRIBUTE_NORMAL;

		m_fileHandle = CreateFile (path.c_str (), accessMode, shareMode, nullptr, creationDisposition, flags | attributes, nullptr);

		if (m_fileHandle == INVALID_HANDLE_VALUE)
		{
			m_fileHandle = nullptr;

			return false;
		}

		m_bOpen = true;
		m_bRead = bRead;
		m_bWrite = bWrite;

		return true;
	}

	void WindowsFile::Close ()
	{
		if (IsOpen ())
		{
			if (CloseHandle (m_fileHandle) == TRUE)
			{
				m_fileHandle = NULL;
				m_bOpen = false;
				m_bRead = false;
				m_bWrite = false;
			}
		}
	}

	int64 WindowsFile::Read (void* buffer, int64 readSize)
	{
		int64 result = 0;

		if (IsRead ())
		{
			DWORD bytesToRead = static_cast<DWORD> (readSize);
			DWORD bytesRead;

			if (ReadFile (m_fileHandle, buffer, bytesToRead, &bytesRead, nullptr) == TRUE)
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
			DWORD bytesToWrite = static_cast<DWORD> (writeSize);
			DWORD bytesWritten;

			if (WriteFile (m_fileHandle, buffer, bytesToWrite, &bytesWritten, nullptr) == TRUE)
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
			LARGE_INTEGER size;

			if (GetFileSizeEx (m_fileHandle, &size) == TRUE)
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

		LARGE_INTEGER zero;
		zero.QuadPart = 0;

		LARGE_INTEGER current;

		if (SetFilePointerEx (m_fileHandle, zero, &current, FILE_CURRENT) == TRUE)
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

		return SetFilePointerEx (m_fileHandle, move, nullptr, moveMethod) == TRUE;
	}
}
