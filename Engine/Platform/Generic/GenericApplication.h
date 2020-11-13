#pragma once

#include <string>

#include "EngineType.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	namespace Platform
	{
		class PLATFORM_API GenericApplication
		{
		public:
			GenericApplication () :
				m_bKeepRunning (true)
			{}
			virtual ~GenericApplication () = 0 {}

			virtual bool Initialize (const std::wstring& name, uint32 width, uint32 height, bool bFullScreenEnabled) = 0;
			virtual void Shutdown () = 0;

			virtual bool Update () = 0;
			virtual void RequestExit ();
			virtual void ResizeWindow (uint32 width, uint32 height, bool bFullScreen) = 0;

			virtual void* GetNativeWindowHandle () const = 0;
			virtual std::wstring GetPath () const = 0;

		protected:
			bool m_bKeepRunning;
		};
	}
}
