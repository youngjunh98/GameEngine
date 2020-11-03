#include "Platform/Windows/WindowsFile.h"

namespace GameEngine
{
	WindowsFile::WindowsFile ()
	{
	}

	WindowsFile::~WindowsFile ()
	{
		if (m_file != NULL && m_file != INVALID_HANDLE_VALUE)
		{
			CloseHandle (m_file);
		}
	}

	bool WindowsFile::Open (const std::wstring& path, bool bRead, bool bWrite)
	{
		if (IsOpen ())
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

		m_file = CreateFile (path.c_str (), accessMode, shareMode, nullptr, creationDisposition, flags | attributes, nullptr);

		if (m_file == INVALID_HANDLE_VALUE)
		{
			wchar_t *p_error_message;
			// GetLastError의 오류 코드 값에 대한 설명을 문자열 형태로 만든다. 
			// 이 설명은 동적으로 메모리 할당된 메 모리에 저장되며 
			//그 메모리의 주소는 p_error_message에 저장됩니다.
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, GetLastError (), MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &p_error_message, 0, NULL);
			std::wstring msg (p_error_message);
			m_file = nullptr;
			LocalFree (p_error_message);

			return false;
		}

		m_bOpen = true;
		m_bRead = bRead;
		m_bWrite = bWrite;

		return true;
	}

	void WindowsFile::Close ()
	{
		if (IsOpen () == false)
		{
			return;
		}

		CloseHandle (m_file);

		m_file = NULL;
		m_bOpen = false;
		m_bRead = false;
		m_bWrite = false;
	}

	void WindowsFile::Read (void* buffer, int64 startOffset, int64 readSize)
	{
		if (IsOpen () == false || m_bRead == false)
		{
			return;
		}
		
		LARGE_INTEGER pointer;
		pointer.QuadPart = startOffset;

		if (SetFilePointerEx (m_file, pointer, nullptr, FILE_BEGIN) == FALSE)
		{
			return;
		}

		if (ReadFile (m_file, buffer, static_cast<DWORD> (readSize), nullptr, nullptr) == FALSE)
		{
			return;
		}
	}

	void WindowsFile::ReadAll (void* buffer, int64 bufferSize)
	{
		if (IsOpen () == false || m_bRead == false)
		{
			return;
		}

		int64 fileSize = GetSize ();
		int64 maxReadSize = fileSize > bufferSize ? bufferSize : fileSize;

		Read (buffer, 0, maxReadSize);
	}

	void WindowsFile::Write (const void* buffer, int64 bufferSize)
	{
		if (IsOpen () == false || m_bWrite == false)
		{
			return;
		}

		WriteFile (m_file, buffer, bufferSize, nullptr, nullptr);
	}

	int64 WindowsFile::GetSize () const
	{
		if (IsOpen () == false)
		{
			return 0;
		}

		LARGE_INTEGER size;

		if (GetFileSizeEx (m_file, &size) == FALSE)
		{
			return 0;
		}

		return size.QuadPart;
	}
}
