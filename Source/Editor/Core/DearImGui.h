#pragma once

#include "Engine/Platform/PlatformMacro.h"
#include "Editor/Core/EditorCoreMacro.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#if defined(ENGINE_PLATFORM_WINDOWS)
    #include <Windows.h>
    #include <imgui_impl_win32.h>
    #include <imgui_impl_dx11.h>

    // Forward declare message handler from imgui_impl_win32.cpp
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

namespace GameEngine
{
    class EDITOR_CORE_API DearImGui final
    {
    public:
        static bool Initialize ();
        static void CleanUp ();
        
        static void StartRender ();
        static void FinishRender ();

        static bool BeginMainMenuBar ();
        static void EndMainMenuBar ();

    private:
        DearImGui () {}
        ~DearImGui () {}
    };
}
