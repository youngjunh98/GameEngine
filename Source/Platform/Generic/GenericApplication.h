#pragma once

#include <string>
#include <vector>

#include "Type.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	namespace Platform
	{
		using ApplicationResizeCallback = void (*) (uint32 width, uint32 height);

		class GenericApplication
		{
		public:
			PLATFORM_API GenericApplication ();
			PLATFORM_API virtual ~GenericApplication () = 0;

			PLATFORM_API virtual bool Initialize (const std::wstring& name, uint32 width, uint32 height, bool bFullScreenEnabled) = 0;
			PLATFORM_API virtual void Shutdown () = 0;

			PLATFORM_API virtual bool Update () = 0;
			PLATFORM_API virtual void RequestQuit () = 0;

			PLATFORM_API virtual void ResizeWindow (uint32 width, uint32 height, bool bFullScreen) = 0;
			PLATFORM_API void AddResizeListener (ApplicationResizeCallback callback);
			PLATFORM_API void RemoveResizeListener (ApplicationResizeCallback callback);
			PLATFORM_API void ExecuteResizeCallbacks(uint32 width, uint32 height);

			PLATFORM_API virtual void* GetNativeWindowHandle () const = 0;
			PLATFORM_API virtual std::wstring GetPath () const = 0;

		private:
			std::vector<ApplicationResizeCallback> m_resizeListeners;
		};
	}
}
