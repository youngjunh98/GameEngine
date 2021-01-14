#include <tchar.h>

#include "Engine/Platform/Windows/WindowsApplication.h"
#include "Engine/Platform/Platform.h"

namespace GameEngine
{
	namespace Platform
	{
		bool WindowsApplication::Initialize (const std::wstring& name, uint32 width, uint32 height, bool bFullScreenEnabled)
		{
			m_hInstance = GetModuleHandle (nullptr);

			size_t classNameLength = _tcslen (name.c_str ());
			m_className = std::make_unique<TCHAR[]> (classNameLength + 1);

			_tcsnccpy_s (m_className.get (), classNameLength + 1, name.c_str (), classNameLength);

			WNDCLASS wc = { };
			wc.hInstance = m_hInstance;
			wc.lpfnWndProc = WndProc;
			wc.lpszClassName = name.c_str ();
			wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
			wc.hCursor = LoadCursor (nullptr, IDC_ARROW);
			wc.hIcon = LoadIcon (nullptr, IDI_WINLOGO);

			if (RegisterClass (&wc) == false)
			{
				return false;
			}

			DWORD style = 0;

			if (bFullScreenEnabled)
			{
				width = GetSystemMetrics (SM_CXSCREEN);
				height = GetSystemMetrics (SM_CYSCREEN);
				style |= WS_POPUP;
			}
			else
			{
				style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX;
			}

			UINT screenWidth = GetSystemMetrics (SM_CXSCREEN);
			UINT screenHeight = GetSystemMetrics (SM_CYSCREEN);

			RECT windowRect = { };
			windowRect.left = (screenWidth / 2) - (width / 2);
			windowRect.top = (screenHeight / 2) - (height / 2);
			windowRect.right = (screenWidth / 2) + (width / 2);
			windowRect.bottom = (screenHeight / 2) + (height / 2);

			AdjustWindowRect (&windowRect, style, false);

			UINT windowX = windowRect.left;
			UINT windowY = windowRect.top;
			UINT windowWidth = windowRect.right - windowRect.left;
			UINT windowHeight = windowRect.bottom - windowRect.top;

			m_hWnd = CreateWindow (
				name.c_str (),
				name.c_str (),
				style,
				windowX,
				windowY,
				windowWidth,
				windowHeight,
				nullptr,
				nullptr,
				m_hInstance,
				nullptr
			);

			if (m_hWnd == nullptr)
			{
				return false;
			}

			ShowWindow (m_hWnd, SW_SHOW);

			return true;
		}

		void WindowsApplication::Shutdown ()
		{
			m_hWnd = nullptr;

			UnregisterClass (m_className.get (), m_hInstance);
			m_hInstance = nullptr;
		}

		bool WindowsApplication::Update ()
		{
			MSG message = { };

			while (PeekMessage (&message, nullptr, 0, 0, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
				{
					return false;
				}

				TranslateMessage (&message);
				DispatchMessage (&message);
			}

			return true;
		}

		void WindowsApplication::RequestQuit ()
		{
			PostMessage (m_hWnd, WM_CLOSE, 0, 0);
		}

		void WindowsApplication::ResizeWindow (uint32 width, uint32 height, bool bFullScreenEnabled)
		{
			LONG_PTR style = WS_VISIBLE;

			if (bFullScreenEnabled)
			{
				width = GetSystemMetrics (SM_CXSCREEN);
				height = GetSystemMetrics (SM_CYSCREEN);
				style |= WS_POPUP;
			}
			else
			{
				style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX;
			}

			UINT screenWidth = GetSystemMetrics (SM_CXSCREEN);
			UINT screenHeight = GetSystemMetrics (SM_CYSCREEN);

			RECT windowRect = { };
			windowRect.left = (screenWidth / 2) - (width / 2);
			windowRect.top = (screenHeight / 2) - (height / 2);
			windowRect.right = (screenWidth / 2) + (width / 2);
			windowRect.bottom = (screenHeight / 2) + (height / 2);

			AdjustWindowRect (&windowRect, static_cast<DWORD> (style), false);

			UINT windowX = windowRect.left;
			UINT windowY = windowRect.top;
			UINT windowWidth = windowRect.right - windowRect.left;
			UINT windowHeight = windowRect.bottom - windowRect.top;

			SetWindowLongPtr (m_hWnd, GWL_STYLE, style);
			SetWindowPos (m_hWnd, nullptr, windowX, windowY, windowWidth, windowHeight, SWP_FRAMECHANGED);
		}

		void* WindowsApplication::GetNativeWindowHandle () const
		{
			return m_hWnd;
		}

		PathString WindowsApplication::GetPath () const
		{
			TCHAR path[MAX_PATH];
			GetModuleFileName (nullptr, path, MAX_PATH);

			return PathString (path);
		}

		void WindowsApplication::AddWindowProcedureListener (WindowProcedure callback)
		{
			m_windowProcedureCallbacks.push_back (callback);
		}

		void WindowsApplication::RemoveWindowProcedureListener (WindowProcedure callback)
		{
			for (auto it = m_windowProcedureCallbacks.begin (); it != m_windowProcedureCallbacks.end ();)
			{
				if (*it == callback)
				{
					it = m_windowProcedureCallbacks.erase (it);
				}
				else
				{
					++it;
				}
			}
		}

		void WindowsApplication::ExecuteWindowProcedureCallbacks (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			for (auto callback : m_windowProcedureCallbacks)
			{
				callback (hWnd, message, wParam, lParam);
			}
		}
	}
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GameEngine::Platform::GenericApplication& application = GameEngine::Platform::GetGenericApplication ();
	GameEngine::Platform::GenericInput& input = GameEngine::Platform::GetGenericInput ();

	auto& windowsApplication = static_cast<GameEngine::Platform::WindowsApplication&> (application);
	auto& windowsInput = static_cast<GameEngine::Platform::WindowsInput&> (input);

	switch (message)
	{
		case WM_CLOSE:
		{
			DestroyWindow (hWnd);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage (0);
			return 0;
		}

		case WM_SIZE:
		{
			UINT width = LOWORD (lParam);
			UINT height = HIWORD (lParam);
			windowsApplication.ExecuteResizeCallbacks (width, height);
			break;
		}

		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_MOUSEHOVER:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		{
			windowsInput.ProcessMouseMessages (message, wParam, lParam);
			break;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			windowsInput.ProcessKeyboardMessages (message, wParam, lParam);
			break;
		}
	}

	windowsApplication.ExecuteWindowProcedureCallbacks (hWnd, message, wParam, lParam);

	return DefWindowProc (hWnd, message, wParam, lParam);
}
