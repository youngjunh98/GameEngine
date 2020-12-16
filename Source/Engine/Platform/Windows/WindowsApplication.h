#pragma once

#include <Windows.h>
#include <memory>
#include <vector>

#include "Engine/Platform/Generic/GenericApplication.h"

namespace GameEngine
{
	namespace Platform
	{
		using WindowProcedure = LRESULT (*) (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		class WindowsApplication : public GenericApplication
		{
		public:
			ENGINE_PLATFORM_API WindowsApplication () : m_hInstance (nullptr), m_hWnd (nullptr) {}
			ENGINE_PLATFORM_API virtual ~WindowsApplication () {}

			ENGINE_PLATFORM_API virtual bool Initialize (const std::wstring& name, uint32 width, uint32 height, bool bFullScreenEnabled) override;
			ENGINE_PLATFORM_API virtual void Shutdown () override;

			ENGINE_PLATFORM_API virtual bool Update () override;
			ENGINE_PLATFORM_API virtual void RequestQuit () override;
			ENGINE_PLATFORM_API virtual void ResizeWindow (uint32 width, uint32 height, bool bFullScreen) override;

			ENGINE_PLATFORM_API virtual void* GetNativeWindowHandle () const override;
			ENGINE_PLATFORM_API virtual std::wstring GetPath () const override;

			ENGINE_PLATFORM_API void AddWindowProcedureListener (WindowProcedure callback);
			ENGINE_PLATFORM_API void RemoveWindowProcedureListener (WindowProcedure callback);
			ENGINE_PLATFORM_API void ExecuteWindowProcedureCallbacks (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		private:
			HINSTANCE m_hInstance;
			HWND m_hWnd;
			std::unique_ptr<TCHAR[]> m_className;
			std::vector<WindowProcedure> m_windowProcedureCallbacks;
		};
	}
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
