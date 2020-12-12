#include "GameWindow.h"
#include "Editor/EditorGUI.h"
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
        Vector2 desiredSize = EditorGUI::GetWindowAvailableContentRegionSize ();
        Vector2 renderSize = Vector2 (Math::Floor (desiredSize.m_x), Math::Floor (desiredSize.m_y));

        if (renderSize.m_x >= 1.0f && renderSize.m_y >= 1.0f)
        {
            bool bSizeChanged = m_renderSize != renderSize;
            RenderingInterface& renderingInterface = GlobalRenderer::GetRenderingInterface ();
            bool bUpdateRenderTarget = bSizeChanged || m_gameRenderTexture == nullptr;
            bool bUpdateDepthStencil = bSizeChanged || m_gameDepthStencilTexture == nullptr;

            if (bUpdateRenderTarget || bUpdateDepthStencil)
            {
                uint32 widthInt = static_cast<uint32> (renderSize.m_x);
                uint32 heightInt = static_cast<uint32> (renderSize.m_y);

                if (bUpdateRenderTarget)
                {
                    m_gameRenderBuffer = renderingInterface.CreateTexture2D (widthInt, heightInt, 1, 1, ERenderingResourceFormat::R8G8B8A8_UNorm, nullptr, nullptr, false, true, true, false);
                    m_gameRenderTarget = renderingInterface.CreateRenderTargetView (m_gameRenderBuffer.get (), 0, 1, 0);
                    m_gameRenderTexture = renderingInterface.CreateShaderResourceView (m_gameRenderBuffer.get (), ERenderingResourceFormat::R8G8B8A8_UNorm, 0, 1, 0);
                }

                if (bUpdateDepthStencil)
                {
                    m_gameDepthStencilBuffer = renderingInterface.CreateTexture2D (widthInt, heightInt, 1, 1, ERenderingResourceFormat::R24G8_Typeless, nullptr, nullptr, false, true, false, true);
                    m_gameDepthStencil = renderingInterface.CreateDepthStencilView (m_gameDepthStencilBuffer.get (), 0, 1, 0);
                    m_gameDepthStencilTexture = renderingInterface.CreateShaderResourceView (m_gameDepthStencilBuffer.get (), ERenderingResourceFormat::R24_UNorm_X8_Typeless, 0, 1, 0);
                }

                m_renderSize = renderSize;
                GlobalRenderer::SetRenderSize (renderSize);
                GlobalRenderer::SetRenderTarget (m_gameRenderTarget);
                GlobalRenderer::SetDepthStencil (m_gameDepthStencil);
            }
        }

		if (m_gameRenderTexture != nullptr)
		{
            void* nativePointer = m_gameRenderTexture->GetNative ();
            EditorGUI::Image (nativePointer, renderSize);
		}
    }
}
