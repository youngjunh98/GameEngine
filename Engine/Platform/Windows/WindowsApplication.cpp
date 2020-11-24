#include <tchar.h>

#include "Platform/Windows/WindowsApplication.h"
#include "Platform/Platform.h"

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
				style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
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
				style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
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

		std::wstring WindowsApplication::GetPath () const
		{
			TCHAR path[MAX_PATH];
			GetModuleFileName (nullptr, path, MAX_PATH);

			return std::wstring (path);
		}

		void WindowsApplication::AddWindowProcedureCallback (WindowProcedure callback)
		{
			m_windowProcedureCallbacks.push_back (callback);
		}

		void WindowsApplication::ExecuteWindowProcedureCallback (UINT message, WPARAM wParam, LPARAM lParam)
		{
			for (auto callback : m_windowProcedureCallbacks)
			{
				callback (m_hWnd, message, wParam, lParam);
			}
		}
	}
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
			auto& input = static_cast<GameEngine::Platform::Input&> (GameEngine::Platform::GetGenericInput ());
			input.ProcessMouseMessages (message, wParam, lParam);

			break;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			auto& input = static_cast<GameEngine::Platform::Input&> (GameEngine::Platform::GetGenericInput ());
			input.ProcessKeyboardMessages (message, wParam, lParam);

			break;
		}
	}

	auto& windowsApplication = static_cast<GameEngine::Platform::WindowsApplication&> (GameEngine::Platform::GetGenericApplication ());
	windowsApplication.ExecuteWindowProcedureCallback (message, wParam, lParam);

	return DefWindowProc (hWnd, message, wParam, lParam);
}
