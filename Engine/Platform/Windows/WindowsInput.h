#pragma once

#include <Windows.h>

#include "Platform/Generic/GenericInput.h"

namespace GameEngine
{
	namespace Platform
	{
		class PLATFORM_API WindowsInput : public GenericInput
		{
		public:
			WindowsInput () {}
			virtual ~WindowsInput () {}

			virtual bool Initialize () override;
			virtual void Shutdown () override;

			void ProcessKeyboardMessages (UINT message, WPARAM wParam, LPARAM lParam);
			void ProcessMouseMessages (UINT message, WPARAM wParam, LPARAM lParam);

		private:
			EKeyCode MapVirtualKeyToKeyCode (WPARAM virtualKey, bool bExtendedKey);
		};
	}
}
