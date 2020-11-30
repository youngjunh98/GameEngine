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
		
		class PLATFORM_API WindowsApplication : public GenericApplication
		{
		public:
			WindowsApplication () : m_hInstance (nullptr), m_hWnd (nullptr) {}
			virtual ~WindowsApplication () {}

			virtual bool Initialize (const std::wstring& name, uint32 width, uint32 height, bool bFullScreenEnabled) override;
			virtual void Shutdown () override;

			virtual bool Update () override;
			virtual void RequestQuit () override;
			virtual void ResizeWindow (uint32 width, uint32 height, bool bFullScreen) override;

			virtual void* GetNativeWindowHandle () const override;
			virtual std::wstring GetPath () const override;

			void AddWindowProcedureListener (WindowProcedure callback);
			void RemoveWindowProcedureListener (WindowProcedure callback);
			void ExecuteWindowProcedureCallbacks (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		private:
			HINSTANCE m_hInstance;
			HWND m_hWnd;
			std::unique_ptr<TCHAR[]> m_className;
			std::vector<WindowProcedure> m_windowProcedureCallbacks;
		};
	}
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
