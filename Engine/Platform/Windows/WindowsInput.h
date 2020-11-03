#pragma once

#include <Windows.h>

#include "Platform/Generic/GenericPlatformInput.h"

namespace GameEngine
{
	class PLATFORM_API WindowsInput : public GenericPlatformInput
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
