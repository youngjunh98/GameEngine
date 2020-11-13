#pragma once

#include "PlatformMacro.h"

// Generic Platform System
#include "Generic/GenericApplication.h"
#include "Generic/GenericInput.h"
#include "Generic/GenericTimer.h"

// Generic Platform File
#include "Generic/GenericFile.h"
#include "Generic/GenericFileSystem.h"

// Generic Platform Thread
#include "Generic/GenericThread.h"

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
		namespace Platform
		{
			using Application = WindowsApplication;
			using Input = WindowsInput;
			using Timer = WindowsTimer;
			using FileSystem = WindowsFileSystem;
			using File = WindowsFile;
			using Thread = WindowsThread;
		}
	}

#endif

namespace GameEngine
{
	namespace Platform
	{
		PLATFORM_API GenericApplication& GetGenericApplication ();
		PLATFORM_API GenericInput& GetGenericInput ();
		PLATFORM_API GenericTimer& GetGenericTimer ();
	}
}