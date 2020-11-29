#include "Platform/Generic/GenericFile.h"

namespace GameEngine
{
	namespace Platform
	{
		GenericFile::GenericFile () :
			m_fileHandle (nullptr), m_bOpen (false), m_accessMode ()
		{
		}

		GenericFile::~GenericFile ()
		{
			m_fileHandle = nullptr;
			m_bOpen = false;
		}

		bool GenericFile::IsOpen () const
		{
			return m_bOpen;
		}

		bool GenericFile::IsRead () const
		{
			return m_bOpen && (m_accessMode == EFileAccessMode::Read || m_accessMode == EFileAccessMode::ReadWrite);
		}

		bool GenericFile::IsWrite () const
		{
			return m_bOpen && (m_accessMode == EFileAccessMode::Write || m_accessMode == EFileAccessMode::ReadWrite);
		}
	}
}