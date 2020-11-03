#include "Platform/Platform.h"

namespace GameEngine
{
	GenericPlatformApplication& GameEngine::Platform::GetGenericApplication ()
	{
		static PlatformApplication platformApplicationSingleton;
		return platformApplicationSingleton;
	}

	GenericPlatformInput& Platform::GetGenericInput ()
	{
		static PlatformInput platformInputSingleton;
		return platformInputSingleton;
	}

	GenericPlatformTimer& Platform::GetGenericTimer ()
	{
		static PlatformTimer platformTimerSingleton;
		return platformTimerSingleton;
	}

	GenericPlatformFileSystem& Platform::GetGenericFileSystem ()
	{
		static PlatformFileSystem platformFileSystemSingleton;
		return platformFileSystemSingleton;
	}
}