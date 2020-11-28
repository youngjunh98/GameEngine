#pragma once

#include <string>
#include <vector>

#include "EngineType.h"
#include "Platform/PlatformMacro.h"

namespace GameEngine
{
	namespace Platform
	{
		using ApplicationResizeCallback = void (*) (uint32 width, uint32 height);

		class PLATFORM_API GenericApplication
		{
		public:
			GenericApplication ();
			virtual ~GenericApplication () = 0;

			virtual bool Initialize (const std::wstring& name, uint32 width, uint32 height, bool bFullScreenEnabled) = 0;
			virtual void Shutdown () = 0;

			virtual bool Update () = 0;
			virtual void RequestQuit () = 0;

			virtual void ResizeWindow (uint32 width, uint32 height, bool bFullScreen) = 0;
			void AddResizeListener (ApplicationResizeCallback callback);
			void RemoveResizeListener (ApplicationResizeCallback callback);
			void ExecuteResizeCallbacks(uint32 width, uint32 height);

			virtual void* GetNativeWindowHandle () const = 0;
			virtual std::wstring GetPath () const = 0;

		private:
			std::vector<ApplicationResizeCallback> m_resizeListeners;
		};
	}
}
