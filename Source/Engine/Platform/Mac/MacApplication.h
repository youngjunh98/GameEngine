#pragma once

#include <memory>
#include <vector>

#include "Engine/Platform/Generic/GenericApplication.h"

namespace GameEngine
{
	namespace Platform
	{
		class MacApplication : public GenericApplication
		{
		public:
			ENGINE_PLATFORM_API MacApplication ();
			ENGINE_PLATFORM_API virtual ~MacApplication ();

			ENGINE_PLATFORM_API virtual bool Initialize (const PathString& name, uint32 width, uint32 height, bool bFullScreenEnabled) override;
			ENGINE_PLATFORM_API virtual void Shutdown () override;

			ENGINE_PLATFORM_API virtual bool Update () override;
			ENGINE_PLATFORM_API virtual void RequestQuit () override;
			ENGINE_PLATFORM_API virtual void ResizeWindow (uint32 width, uint32 height, bool bFullScreen) override;

			ENGINE_PLATFORM_API virtual void* GetNativeWindowHandle () const override;
			ENGINE_PLATFORM_API virtual PathString GetExecutablePath () const override;

		private:
		};
	}
}
