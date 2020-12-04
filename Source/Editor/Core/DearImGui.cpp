#include "DearImGui.h"

#include <imgui.cpp>
#include <imgui_draw.cpp>
#include <imgui_widgets.cpp>
#include <misc/cpp/imgui_stdlib.cpp>

#if defined(PLATFORM_WINDOWS)
    #include <imgui_impl_win32.cpp>
    #include <imgui_impl_dx11.cpp>
#endif

#include "Platform/Platform.h"
#include "Engine/Rendering/GlobalRenderer.h"

namespace GameEngine
{
    bool DearImGui::Initialize ()
    {
        // Setup Dear ImGui context
		IMGUI_CHECKVERSION ();
		ImGui::CreateContext ();
		ImGuiIO& io = ImGui::GetIO (); (void) io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark ();
		ImGuiStyle& style = ImGui::GetStyle ();
		style.WindowRounding = 1.0f;

        // Setup Platform/Renderer bindings
#if defined(PLATFORM_WINDOWS)
		auto& windowsApplication = static_cast<Platform::WindowsApplication&> (Platform::GetGenericApplication ());
        HWND hWnd = static_cast<HWND> (windowsApplication.GetNativeWindowHandle ());

        RenderingInterface& renderingInterface = g_renderer.GetRenderingInterface ();
		auto* d3d11Device = reinterpret_cast<ID3D11Device*> (renderingInterface.GetNativeDevice ());
		auto* d3d11Context = reinterpret_cast<ID3D11DeviceContext*> (renderingInterface.GetNativeContext ());

        if (ImGui_ImplWin32_Init(hWnd) == false)
        {
            return false;
        }

        if (ImGui_ImplDX11_Init (d3d11Device, d3d11Context) == false)
        {
            return false;
        }

        windowsApplication.AddWindowProcedureListener (ImGui_ImplWin32_WndProcHandler);
#endif

        return true;
    }

    void DearImGui::CleanUp ()
    {
#if defined(PLATFORM_WINDOWS)
        auto& winApp = static_cast<Platform::WindowsApplication&> (Platform::GetGenericApplication ());
        winApp.RemoveWindowProcedureListener (ImGui_ImplWin32_WndProcHandler);

        ImGui_ImplDX11_Shutdown ();
        ImGui_ImplWin32_Shutdown ();
#endif

        ImGui::DestroyContext ();
    }

    void DearImGui::StartRender ()
    {
#if defined(PLATFORM_WINDOWS)
        ImGui_ImplDX11_NewFrame ();
        ImGui_ImplWin32_NewFrame ();
#endif

        ImGui::NewFrame ();
    }

    void DearImGui::FinishRender ()
    {
        ImGui::Render ();

#if defined(PLATFORM_WINDOWS)
        ImGui_ImplDX11_RenderDrawData (ImGui::GetDrawData ());
#endif
    }

    bool DearImGui::BeginMainMenuBar ()
    {
        return ImGui::BeginMainMenuBar ();
    }

    void DearImGui::EndMainMenuBar ()
    {
        ImGui::EndMainMenuBar ();
    }
}
