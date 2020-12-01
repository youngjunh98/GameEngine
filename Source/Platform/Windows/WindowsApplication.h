#pragma once

#include <Windows.h>
#include <memory>
#include <vector>

#include "Platform/Generic/GenericApplication.h"

namespace GameEngine
{
	namespace Platform
	{
		using WindowProcedure = LRESULT (*) (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		class WindowsApplication : public GenericApplication
		{
		public:
			PLATFORM_API WindowsApplication () : m_hInstance (nullptr), m_hWnd (nullptr) {}
			PLATFORM_API virtual ~WindowsApplication () {}

			PLATFORM_API virtual bool Initialize (const std::wstring& name, uint32 width, uint32 height, bool bFullScreenEnabled) override;
			PLATFORM_API virtual void Shutdown () override;

			PLATFORM_API virtual bool Update () override;
			PLATFORM_API virtual void RequestQuit () override;
			PLATFORM_API virtual void ResizeWindow (uint32 width, uint32 height, bool bFullScreen) override;

			PLATFORM_API virtual void* GetNativeWindowHandle () const override;
			PLATFORM_API virtual std::wstring GetPath () const override;

			PLATFORM_API void AddWindowProcedureListener (WindowProcedure callback);
			PLATFORM_API void RemoveWindowProcedureListener (WindowProcedure callback);
			PLATFORM_API void ExecuteWindowProcedureCallbacks (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		private:
			HINSTANCE m_hInstance;
			HWND m_hWnd;
			std::unique_ptr<TCHAR[]> m_className;
			std::vector<WindowProcedure> m_windowProcedureCallbacks;
		};
	}
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
