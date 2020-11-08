#pragma once

#include "PlatformMacro.h"

// Generic Platform System
#include "Generic/GenericPlatformApplication.h"
#include "Generic/GenericPlatformInput.h"
#include "Generic/GenericPlatformTimer.h"

// Generic Platform File
#include "Generic/GenericPlatformFile.h"
#include "Generic/GenericPlatformFileSystem.h"

// Generic Platform Thread
#include "Generic/GenericPlatformThread.h"

#if defined (PLATFORM_WINDOWS)

	// Windows Platform System
	#include "Windows/WindowsApplication.h"
	#include "Windows/WindowsInput.h"
	#include "Windows/WindowsTimer.h"

	// Windows Platform File
	#include "Windows/WindowsFileSystem.h"
	#include "Windows/WindowsFile.h"

	// Windows Platform Thread
	#include "Windows/WindowsThread.h"

	namespace GameEngine
	{
		using PlatformApplication = WindowsApplication;
		using PlatformInput = WindowsInput;
		using PlatformTimer = WindowsTimer;
		using PlatformFileSystem = WindowsFileSystem;
		using PlatformFile = WindowsFile;
		using PlatformThread = WindowsThread;
	}

#endif

namespace GameEngine
{
	class PLATFORM_API Platform
	{
	public:
		static GenericPlatformApplication& GetGenericApplication ();
		static GenericPlatformInput& GetGenericInput ();
		static GenericPlatformTimer& GetGenericTimer ();
		static GenericPlatformFileSystem& GetGenericFileSystem ();
	};
}