#include <string>
#include <vector>

#include "ViewportWindow.h"
#include "Editor/EditorGUI.h"
#include "Core/Math/Matrix4x4.h"
#include "Core/Input/Input.h"
#include "Core/Time/Timer.h"
#include "RI/RenderingInterface.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Rendering/Renderer.h"
#include "Engine/Component/Rendering/Light.h"
#include "Engine/Rendering/GlobalRenderer.h"
#include "Engine/Scene/SceneManager.h"

namespace GameEngine
{
    static const std::vector<std::string> g_renderPipelineNames = { "Default", "Wireframe" };

    ViewportWindow::ViewportWindow () : EditorWindow ("Viewport"), m_cameraPosition (Vector3::Zero), m_cameraAngles (Vector3::Zero), m_bMoveCamera (false),
        m_renderPipeline (0), m_renderSize (Vector2::Zero)
    {
    }

    ViewportWindow::~ViewportWindow ()
    {
    }

    void ViewportWindow::OnRender ()
    {
        RenderingInterface& renderingInterface = GlobalRenderer::GetRenderingInterface ();
        RenderPipeline* renderPipeline = nullptr;

        m_renderPipeline = EditorGUI::InputDropDown ("##RenderMode", m_renderPipeline, g_renderPipelineNames);

        if (m_renderPipeline == 0)
        {
            renderPipeline = GlobalRenderer::GetDefaultForwardRenderPipeline ();

        }
        else if (m_renderPipeline == 1)
        {
            renderPipeline = GlobalRenderer::GetDefaultLineRenderPipeline ();
        }

        Vector2 desiredSize = EditorGUI::GetWindowAvailableContentRegionSize ();
        Vector2 renderSize = Vector2 (Math::Floor (desiredSize.m_x), Math::Floor (desiredSize.m_y));

        if (renderSize.m_x >= 1.0f && renderSize.m_y >= 1.0f)
        {
            bool bSizeChanged = m_renderSize != renderSize;
            bool bUpdateRenderTarget = bSizeChanged || m_renderTexture == nullptr;
            bool bUpdateDepthStencil = bSizeChanged || m_depthStencilTexture == nullptr;

            if (bUpdateRenderTarget || bUpdateDepthStencil)
            {
                uint32 widthInt = static_cast<uint32> (renderSize.m_x);
                uint32 heightInt = static_cast<uint32> (renderSize.m_y);

                if (bUpdateRenderTarget)
                {
                    m_renderBuffer = renderingInterface.CreateTexture2D (widthInt, heightInt, 1, 1, ERenderingResourceFormat::R8G8B8A8_UNorm, nullptr, nullptr, false, true, true, false);
                    m_renderTarget = renderingInterface.CreateRenderTargetView (m_renderBuffer.get (), 0, 1, 0);
                    m_renderTexture = renderingInterface.CreateShaderResourceView (m_renderBuffer.get (), ERenderingResourceFormat::R8G8B8A8_UNorm, 0, 1, 0);
                }

                if (bUpdateDepthStencil)
                {
                    m_depthStencilBuffer = renderingInterface.CreateTexture2D (widthInt, heightInt, 1, 1, ERenderingResourceFormat::R24G8_Typeless, nullptr, nullptr, false, true, false, true);
                    m_depthStencil = renderingInterface.CreateDepthStencilView (m_depthStencilBuffer.get (), 0, 1, 0);
                    m_depthStencilTexture = renderingInterface.CreateShaderResourceView (m_depthStencilBuffer.get (), ERenderingResourceFormat::R24_UNorm_X8_Typeless, 0, 1, 0);
                }

                m_renderSize = renderSize;
            }
        }

        Quaternion cameraRotation = Quaternion::FromEuler (m_cameraAngles);
        Vector3 cameraForward = cameraRotation * Vector3::Forward;
        Vector3 cameraRight = cameraRotation * Vector3::Right;
        Vector3 cameraUp = cameraRotation * Vector3::Up;

        renderingInterface.ClearRenderTarget (m_renderTarget.get (), 0.0f, 0.0f, 0.0f, 1.0f);
        renderingInterface.ClearDepthStencil (m_depthStencil.get (), 1.0f, 0);

        if (renderPipeline != nullptr)
        {
            Scene* scene = g_sceneManager.GetScene ();

            if (scene != nullptr)
            {
                Vector2 oldSize = GlobalRenderer::GetRenderSize ();
                RenderingResourcePtr<RI_RenderTargetView> oldRenderTarget = GlobalRenderer::GetRenderTarget ();
                RenderingResourcePtr<RI_DepthStencilView> oldDepthStencil = GlobalRenderer::GetDepthStencil ();

                GlobalRenderer::SetRenderSize (m_renderSize);
                GlobalRenderer::SetRenderTarget (m_renderTarget);
                GlobalRenderer::SetDepthStencil (m_depthStencil);

                float fov = 60.0f;
                float aspect = renderSize.m_x / renderSize.m_y;
                float cameraNear = 0.001f;
                float cameraFar = 1000.0f;
                Matrix4x4 view = Matrix4x4::LookAt (m_cameraPosition, m_cameraPosition + cameraForward, cameraUp);
                Matrix4x4 projection = Matrix4x4::Perspective (fov, aspect, cameraNear, cameraFar);

                GlobalRenderer::RenderScene (*renderPipeline, view, projection, m_cameraPosition, cameraNear, cameraFar, scene);

                GlobalRenderer::SetRenderSize (oldSize);
                GlobalRenderer::SetRenderTarget (oldRenderTarget);
                GlobalRenderer::SetDepthStencil (oldDepthStencil);
            }
        }

        if (m_renderTexture == nullptr)
		{
            return;
		}

        void* nativePointer = m_renderTexture->GetNative ();
        EditorGUI::Image (nativePointer, renderSize);

        if (Input::GetMouseButtonUp (EMouseButton::Right))
        {
            m_bMoveCamera = false;
        }

        if (EditorGUI::IsWindowFocused () == false)
        {
            return;
        }

        if (EditorGUI::IsLastItemHovered () && Input::GetMouseButtonDown (EMouseButton::Right))
        {
            m_bMoveCamera = true;
        }

        if (m_bMoveCamera)
        {
            float moveSpeed = 50.0f * Timer::DeltaTime ();
            float rotateSpeed = 360.0f * Timer::DeltaTime ();

            if (Input::GetKey (EKeyCode::W))
            {
                m_cameraPosition += cameraForward * moveSpeed;
            }

            if (Input::GetKey (EKeyCode::S))
            {
                m_cameraPosition -= cameraForward * moveSpeed;
            }

            if (Input::GetKey (EKeyCode::D))
            {
                m_cameraPosition += cameraRight * moveSpeed;
            }

            if (Input::GetKey (EKeyCode::A))
            {
                m_cameraPosition -= cameraRight * moveSpeed;
            }

            Vector2 mouseMoved = Input::GetMouseDelta ();
            m_cameraAngles.m_x += mouseMoved.m_y;
            m_cameraAngles.m_y += mouseMoved.m_x;
        }
    }
}
