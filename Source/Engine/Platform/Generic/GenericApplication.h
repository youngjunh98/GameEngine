#pragma once

#include <string>
#include <vector>

#include "Type.h"
#include "Engine/Platform/PlatformMacro.h"
#include "Engine/Platform/Generic/GenericFileSystem.h"

namespace GameEngine
{
	namespace Platform
	{
		using ApplicationResizeCallback = void (*) (uint32 width, uint32 height);

		class GenericApplication
		{
		public:
			ENGINE_PLATFORM_API GenericApplication ();
			ENGINE_PLATFORM_API virtual ~GenericApplication () = 0;

			ENGINE_PLATFORM_API virtual bool Initialize (const PathString& name, uint32 width, uint32 height, bool bFullScreenEnabled) = 0;
			ENGINE_PLATFORM_API virtual void Shutdown () = 0;

			ENGINE_PLATFORM_API virtual bool Update () = 0;
			ENGINE_PLATFORM_API virtual void RequestQuit () = 0;

			ENGINE_PLATFORM_API virtual void ResizeWindow (uint32 width, uint32 height, bool bFullScreen) = 0;
			ENGINE_PLATFORM_API void AddResizeListener (ApplicationResizeCallback callback);
			ENGINE_PLATFORM_API void RemoveResizeListener (ApplicationResizeCallback callback);
			ENGINE_PLATFORM_API void ExecuteResizeCallbacks(uint32 width, uint32 height);

			ENGINE_PLATFORM_API virtual void* GetNativeWindowHandle () const = 0;
			ENGINE_PLATFORM_API virtual PathString GetExecutablePath () const = 0;

		private:
			std::vector<ApplicationResizeCallback> m_resizeListeners;
		};
	}
}
