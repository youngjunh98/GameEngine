#include "MacApplication.h"
#include "Engine/Platform/Platform.h"

namespace GameEngine
{
	namespace Platform
	{
        MacApplication::MacApplication ()
        {}

        MacApplication::~MacApplication ()
        {}

		bool MacApplication::Initialize (const PathString& name, uint32 width, uint32 height, bool bFullScreenEnabled)
		{
			return true;
		}

		void MacApplication::Shutdown ()
		{
		}

		bool MacApplication::Update ()
		{
			return true;
		}

		void MacApplication::RequestQuit ()
		{
		}

		void MacApplication::ResizeWindow (uint32 width, uint32 height, bool bFullScreenEnabled)
		{
		}

		void* MacApplication::GetNativeWindowHandle () const
		{
			return nullptr;
		}

		PathString MacApplication::GetExecutablePath () const
		{
			return PathString ();
		}
	}
}
