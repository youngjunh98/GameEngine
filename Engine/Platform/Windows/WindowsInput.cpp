#include "Platform/Windows/WindowsInput.h"
#include "Platform/Platform.h"

namespace GameEngine
{
	namespace Platform
	{
		bool WindowsInput::Initialize ()
		{
			RAWINPUTDEVICE rawInputDevice;
			rawInputDevice.usUsagePage = 0x01;
			rawInputDevice.usUsage = 0x02;
			rawInputDevice.dwFlags = 0;
			rawInputDevice.hwndTarget = reinterpret_cast<HWND> (Platform::GetGenericApplication ().GetNativeWindowHandle ());

			if (RegisterRawInputDevices (&rawInputDevice, 1, sizeof (RAWINPUTDEVICE)) == FALSE)
			{
				return false;
			}

			return true;
		}

		void WindowsInput::Shutdown ()
		{
			// Do nothing
		}

		void WindowsInput::ProcessKeyboardMessages (UINT message, WPARAM wParam, LPARAM lParam)
		{
			bool bKeyDown = false;

			switch (message)
			{
				case WM_KEYDOWN:
				case WM_SYSKEYDOWN:
				{
					bKeyDown = true;
					break;
				}

				case WM_KEYUP:
				case WM_SYSKEYUP:
				{
					// Do nothing
					break;
				}

				default: return;
			}

			WPARAM virtualKey = wParam;
			
			if (virtualKey == VK_SHIFT)
			{
				UINT scanCode = (static_cast<UINT> (lParam) & 0x00ff0000) >> 16;
				virtualKey = MapVirtualKey (scanCode, MAPVK_VSC_TO_VK_EX);
			}

			bool bExtendedKey = (static_cast<UINT> (lParam) & 0x01000000) != 0;
			EKeyCode keyCode = MapVirtualKeyToKeyCode (virtualKey, bExtendedKey);
			
			if (bKeyDown)
			{
				KeyDown (keyCode);
			}
			else
			{
				KeyUp (keyCode);
			}
		}

		void WindowsInput::ProcessMouseMessages (UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
				case WM_INPUT:
				{
					RAWINPUT rawInput;
					UINT rawInputSize = sizeof (RAWINPUT);

					if (GetRawInputData (reinterpret_cast<HRAWINPUT> (lParam), RID_INPUT, &rawInput, &rawInputSize, sizeof (RAWINPUTHEADER)) == UINT (-1))
					{
						// When failed to get raw input data.
						// Currently, do nothing.
					}

					if (rawInput.header.dwType == RIM_TYPEMOUSE)
					{
						float mouseDataX = static_cast<float> (rawInput.data.mouse.lLastX);
						float mouseDataY = static_cast<float> (rawInput.data.mouse.lLastY);

						if (rawInput.data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
						{
							MouseDelta (mouseDataX, mouseDataY);
						}
						else if (rawInput.data.mouse.usFlags == MOUSE_MOVE_ABSOLUTE)
						{
							MousePosition (mouseDataX, mouseDataY);
						}
					}

					break;
				}

				case WM_MOUSEMOVE:
				{
					// Do nothing
					break;
				}

				case WM_LBUTTONDOWN:
				{
					MouseButtonDown (EMouseButton::Left);
					break;
				}

				case WM_LBUTTONUP:
				{
					MouseButtonUp (EMouseButton::Left);
					break;
				}

				case WM_RBUTTONDOWN:
				{
					MouseButtonDown (EMouseButton::Right);
					break;
				}

				case WM_RBUTTONUP:
				{
					MouseButtonUp (EMouseButton::Right);
					break;
				}

				case WM_MBUTTONDOWN:
				{
					MouseButtonDown (EMouseButton::Middle);
					break;
				}

				case WM_MBUTTONUP:
				{
					MouseButtonUp (EMouseButton::Middle);
					break;
				}

				case WM_MOUSEWHEEL:
				{
					float mouseScrollDelta = static_cast<float> (GET_WHEEL_DELTA_WPARAM (wParam));
				}

				case WM_XBUTTONDOWN:
				case WM_XBUTTONUP:
				{
					// Do nothing
				}
					break;

				case WM_MOUSEHOVER:
				{
					// Do nothing
				}
					break;

				default: return;
			}
		}

		EKeyCode WindowsInput::MapVirtualKeyToKeyCode (WPARAM virtualKey, bool bExtendedKey)
		{
			switch (virtualKey)
			{
				case VK_ESCAPE:     return EKeyCode::Escape;
				case VK_RETURN:     return bExtendedKey ? EKeyCode::NumpaEnter : EKeyCode::Enter;
				case VK_SPACE:      return EKeyCode::Space;
				case VK_BACK:       return EKeyCode::Backspace;
				case VK_TAB:        return EKeyCode::Tab;
				case VK_CAPITAL:    return EKeyCode::CapsLock;
				case VK_LSHIFT:     return EKeyCode::LeftShift;
				case VK_RSHIFT:     return EKeyCode::RightShift;
				case VK_CONTROL:    return bExtendedKey ? EKeyCode::RightControl : EKeyCode::LeftControl;
				case VK_MENU:       return bExtendedKey ? EKeyCode::RightAlt : EKeyCode::LeftAlt;

				case VK_SNAPSHOT:   return EKeyCode::PrintScreen;
				case VK_SCROLL:     return EKeyCode::ScrollLock;
				case VK_PAUSE:      return EKeyCode::Pause;

				case VK_INSERT:     return EKeyCode::Insert;
				case VK_DELETE:     return EKeyCode::Delete;
				case VK_HOME:       return EKeyCode::Home;
				case VK_END:        return EKeyCode::End;
				case VK_PRIOR:      return EKeyCode::PageUp;
				case VK_NEXT:       return EKeyCode::PageDown;

				case 0x30:          return EKeyCode::Alpha0;
				case 0x31:          return EKeyCode::Alpha1;
				case 0x32:          return EKeyCode::Alpha2;
				case 0x33:          return EKeyCode::Alpha3;
				case 0x34:          return EKeyCode::Alpha4;
				case 0x35:          return EKeyCode::Alpha5;
				case 0x36:          return EKeyCode::Alpha6;
				case 0x37:          return EKeyCode::Alpha7;
				case 0x38:          return EKeyCode::Alpha8;
				case 0x39:          return EKeyCode::Alpha9;

				case 0x41:          return EKeyCode::A;
				case 0x42:          return EKeyCode::B;
				case 0x43:          return EKeyCode::C;
				case 0x44:          return EKeyCode::D;
				case 0x45:          return EKeyCode::E;
				case 0x46:          return EKeyCode::F;
				case 0x47:          return EKeyCode::G;
				case 0x48:          return EKeyCode::H;
				case 0x49:          return EKeyCode::I;
				case 0x4A:          return EKeyCode::J;
				case 0x4B:          return EKeyCode::K;
				case 0x4C:          return EKeyCode::L;
				case 0x4D:          return EKeyCode::M;
				case 0x4E:          return EKeyCode::N;
				case 0x4F:          return EKeyCode::O;
				case 0x50:          return EKeyCode::P;
				case 0x51:          return EKeyCode::Q;
				case 0x52:          return EKeyCode::R;
				case 0x53:          return EKeyCode::S;
				case 0x54:          return EKeyCode::T;
				case 0x55:          return EKeyCode::U;
				case 0x56:          return EKeyCode::V;
				case 0x57:          return EKeyCode::W;
				case 0x58:          return EKeyCode::X;
				case 0x59:          return EKeyCode::Y;
				case 0x5A:          return EKeyCode::Z;

				case VK_OEM_3:      return EKeyCode::BackQuote;
				case VK_OEM_MINUS:  return EKeyCode::Minus;
				case VK_OEM_PLUS:   return EKeyCode::Plus;
				case VK_OEM_5:      return EKeyCode::BackSlash;
				case VK_OEM_4:      return EKeyCode::LeftBracket;
				case VK_OEM_6:      return EKeyCode::RightBracket;
				case VK_OEM_1:      return EKeyCode::Semicolon;
				case VK_OEM_7:      return EKeyCode::Quote;
				case VK_OEM_COMMA:  return EKeyCode::Comma;
				case VK_OEM_PERIOD: return EKeyCode::Dot;
				case VK_OEM_2:      return EKeyCode::Slash;

				case VK_F1:         return EKeyCode::F1;
				case VK_F2:         return EKeyCode::F2;
				case VK_F3:         return EKeyCode::F3;
				case VK_F4:         return EKeyCode::F4;
				case VK_F5:         return EKeyCode::F5;
				case VK_F6:         return EKeyCode::F6;
				case VK_F7:         return EKeyCode::F7;
				case VK_F8:         return EKeyCode::F8;
				case VK_F9:         return EKeyCode::F9;
				case VK_F10:        return EKeyCode::F10;
				case VK_F11:        return EKeyCode::F11;
				case VK_F12:        return EKeyCode::F12;

				case VK_LEFT:       return EKeyCode::LeftArrow;
				case VK_RIGHT:      return EKeyCode::RightArrow;
				case VK_UP:         return EKeyCode::UpArrow;
				case VK_DOWN:       return EKeyCode::DownArrow;

				case VK_NUMPAD0:    return EKeyCode::Numpad0;
				case VK_NUMPAD1:    return EKeyCode::Numpad1;
				case VK_NUMPAD2:    return EKeyCode::Numpad2;
				case VK_NUMPAD3:    return EKeyCode::Numpad3;
				case VK_NUMPAD4:    return EKeyCode::Numpad4;
				case VK_NUMPAD5:    return EKeyCode::Numpad5;
				case VK_NUMPAD6:    return EKeyCode::Numpad6;
				case VK_NUMPAD7:    return EKeyCode::Numpad7;
				case VK_NUMPAD8:    return EKeyCode::Numpad8;
				case VK_NUMPAD9:    return EKeyCode::Numpad9;

				case VK_NUMLOCK:    return EKeyCode::NumLock;
				case VK_DIVIDE:     return EKeyCode::NumpadDivide;
				case VK_MULTIPLY:   return EKeyCode::NumpadMultiply;
				case VK_SUBTRACT:   return EKeyCode::NumpadMinus;
				case VK_ADD:        return EKeyCode::NumpadPlus;
				case VK_DECIMAL:    return EKeyCode::NumpadDot;
			}

			return EKeyCode::None;
		}
	}
}
