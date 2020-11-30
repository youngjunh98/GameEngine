#include "GameWindow.h"
#include "DearImGui.h"
#include "Engine/Rendering/GlobalRenderer.h"
#include "RI/RenderingInterface.h"

namespace GameEngine
{
    GameWindow::GameWindow () : EditorWindow ("Game"), m_renderSize (Vector2::Zero)
    {
    }

    GameWindow::~GameWindow ()
    {
    }

    void GameWindow::OnRender ()
    {
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin ();
		ImVec2 contentMax = ImGui::GetWindowContentRegionMax ();

		float width = Math::Floor (contentMax.x - contentMin.x);
		float height = Math::Floor (contentMax.y - contentMin.y);

        if (width > 0.0f && height > 0.0f)
        {
            Vector2 size (width, height);
            bool bSizeChanged = m_renderSize != size;

            uint32 widthInt = static_cast<uint32> (width);
            uint32 heightInt = static_cast<uint32> (height);

            RenderingInterface& renderingInterface = g_renderer.GetRenderingInterface ();
            bool bRenderTargetChanged = bSizeChanged || m_gameRenderTexture == nullptr;
            bool bDepthStencilChanged = bSizeChanged || m_gameDepthStencilTexture == nullptr;

            if (bRenderTargetChanged)
            {
                m_gameRenderBuffer = renderingInterface.CreateTexture2D (widthInt, heightInt, 1, 1, ERenderingResourceFormat::R8G8B8A8_UNorm, nullptr, nullptr, false, true, true, false);
                m_gameRenderTarget = renderingInterface.CreateRenderTargetView (m_gameRenderBuffer.get (), 0, 1, 0);
                m_gameRenderTexture = renderingInterface.CreateShaderResourceView (m_gameRenderBuffer.get (), ERenderingResourceFormat::R8G8B8A8_UNorm, 0, 1, 0);
            }

            if (bDepthStencilChanged)
            {
                m_gameDepthStencilBuffer = renderingInterface.CreateTexture2D (widthInt, heightInt, 1, 1, ERenderingResourceFormat::R24G8_Typeless, nullptr, nullptr, false, true, false, true);
                m_gameDepthStencil = renderingInterface.CreateDepthStencilView (m_gameDepthStencilBuffer.get (), 0, 1, 0);
                m_gameDepthStencilTexture = renderingInterface.CreateShaderResourceView (m_gameDepthStencilBuffer.get (), ERenderingResourceFormat::R24_UNorm_X8_Typeless, 0, 1, 0);
            }

            if (bRenderTargetChanged || bDepthStencilChanged)
            {
                g_renderer.SetRenderSize (size);
                g_renderer.SetRenderTarget (m_gameRenderTarget);
                g_renderer.SetDepthStencil (m_gameDepthStencil);
            }
        }

        ImTextureID image = nullptr;

		if (m_gameRenderTexture)
		{
			image = m_gameRenderTexture->GetNative ();
		}

		if (image != nullptr)
		{
			ImGui::Image (image, ImVec2 (width, height));
		}
    }
}
