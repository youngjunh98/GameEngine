#include "Platform/Generic/GenericPlatformFile.h"

namespace GameEngine
{
	GenericPlatformFile::GenericPlatformFile () :
		m_bOpen (false)
	{
	}

	GenericPlatformFile::~GenericPlatformFile ()
	{
	}

	bool GenericPlatformFile::IsOpen () const
	{
		return m_bOpen;
	}
}