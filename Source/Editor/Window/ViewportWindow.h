#pragma once

#include "Type.h"
#include "Editor/EditorWindow.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "RI/RenderingInterfaceResource.h"

namespace GameEngine
{
    class ViewportWindow final : public EditorWindow
    {
    public:
        ViewportWindow ();
        virtual ~ViewportWindow ();

        virtual void OnRender () override;

    private:
        Vector3 m_cameraPosition;
        Vector3 m_cameraAngles;
        bool m_bMoveCamera;

        int32 m_renderPipeline;
        Vector2 m_renderSize;
        RenderingResourcePtr<RI_Texture2D> m_renderBuffer;
		RenderingResourcePtr<RI_Texture2D> m_depthStencilBuffer;
		RenderingResourcePtr<RI_RenderTargetView> m_renderTarget;
		RenderingResourcePtr<RI_DepthStencilView> m_depthStencil;
		RenderingResourcePtr<RI_ShaderResourceView> m_renderTexture;
		RenderingResourcePtr<RI_ShaderResourceView> m_depthStencilTexture;
    };
}
