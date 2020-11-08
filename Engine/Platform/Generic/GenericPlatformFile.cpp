#include "Platform/Generic/GenericPlatformFile.h"

namespace GameEngine
{
	GenericPlatformFile::GenericPlatformFile () :
		m_bOpen (false), m_bRead (false), m_bWrite (false)
	{
	}

	GenericPlatformFile::~GenericPlatformFile ()
	{
	}

	bool GenericPlatformFile::IsOpen () const
	{
		return m_bOpen;
	}

	bool GenericPlatformFile::IsRead () const
	{
		return m_bOpen && m_bRead;
	}

	bool GenericPlatformFile::IsWrite () const
	{
		return m_bOpen && m_bWrite;
	}
}