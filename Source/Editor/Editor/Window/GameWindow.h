#pragma once

#include "Type.h"
#include "Editor/Core/EditorWindow.h"
#include "Engine/Core/Math/Vector2.h"
#include "Engine/RI/RenderingInterfaceResource.h"

namespace GameEngine
{
    class GameWindow final : public EditorWindow
    {
    public:
        GameWindow ();
        virtual ~GameWindow ();

        virtual void OnRender () override;

    private:
        Vector2 m_renderSize;
        RenderingResourcePtr<RI_Texture2D> m_gameRenderBuffer;
		RenderingResourcePtr<RI_Texture2D> m_gameDepthStencilBuffer;
		RenderingResourcePtr<RI_RenderTargetView> m_gameRenderTarget;
		RenderingResourcePtr<RI_DepthStencilView> m_gameDepthStencil;
		RenderingResourcePtr<RI_ShaderResourceView> m_gameRenderTexture;
		RenderingResourcePtr<RI_ShaderResourceView> m_gameDepthStencilTexture;
    };
}
