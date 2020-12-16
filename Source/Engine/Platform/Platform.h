#pragma once

#include "PlatformMacro.h"

// Generic Platform System
#include "Generic/GenericApplication.h"
#include "Generic/GenericInput.h"
#include "Generic/GenericTimer.h"
#include "Generic/GenericAudio.h"

// Generic Platform File
#include "Generic/GenericFile.h"
#include "Generic/GenericFileSystem.h"

// Generic Platform Thread
#include "Generic/GenericThread.h"

#if defined (ENGINE_PLATFORM_WINDOWS)

	// Windows Platform System
	#include "Windows/WindowsApplication.h"
	#include "Windows/WindowsInput.h"
	#include "Windows/WindowsTimer.h"
	#include "Windows/WindowsAudio.h"

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
			using Audio = WindowsAudio;
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
		ENGINE_PLATFORM_API GenericApplication& GetGenericApplication ();
		ENGINE_PLATFORM_API GenericInput& GetGenericInput ();
		ENGINE_PLATFORM_API GenericTimer& GetGenericTimer ();
	}
}