#include "Rendering/Pipeline/Line/DefaultRenderLinePass.h"
#include "Rendering/GlobalRenderer.h"
#include "Platform/PlatformRenderingInterface.h"
#include "GameObject.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
#include "Component/Rendering/Light.h"
#include "Component/Rendering/Renderer.h"
#include "Asset/AssetManager.h"

namespace GameEngine
{
	void DefaultRenderLinePass::Release ()
	{
		m_rsLineCullNone = nullptr;
		m_depthLessStencilAlways = nullptr;
		m_whiteMaterial = nullptr;
	}

	void DefaultRenderLinePass::PreRender (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights)
	{
		if (m_rsLineCullNone.get () == nullptr)
		{
			m_rsLineCullNone = g_renderer.GetPlatformRenderingInterface ().CreateRasterizerState (EFillMode::Wireframe, EFaceCulling::None, EWindingOrder::CW);
		}

		if (m_depthLessStencilAlways.get () == nullptr)
		{
			m_depthLessStencilAlways = g_renderer.GetPlatformRenderingInterface ().CreateDepthStencilState (EComparisonFunction::Always, EComparisonFunction::Always);
		}

		g_renderer.GetPlatformRenderingInterface ().SetRasterizerState (m_rsLineCullNone.get ());
		g_renderer.GetPlatformRenderingInterface ().SetDepthStencilState (m_depthLessStencilAlways.get ());

		Vector2 renderSize = g_renderer.GetRenderSize ();
		g_renderer.SetViewport (renderSize);

		g_renderer.BindRenderTargetAndDepthStencil ();
		g_renderer.ClearRenderTargetAndDepthStencil (Vector4 (0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);
	}

	void DefaultRenderLinePass::Render (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights)
	{
		if (cameras.size () <= 0)
		{
			return;
		}

		auto* camera = cameras.at (0);
		CameraConstantBuffer cameraConstantBuffer;
		cameraConstantBuffer.m_view = camera->GetViewMatrix ().Transposed ();
		cameraConstantBuffer.m_projection = camera->GetProjectionMatrix ().Transposed ();
		cameraConstantBuffer.m_viewProjection = cameraConstantBuffer.m_projection * cameraConstantBuffer.m_view;
		cameraConstantBuffer.m_cameraWorldPosition = camera->GetGameObject ().GetTransform ().GetPosition ();
		cameraConstantBuffer.m_cameraFar = camera->GetFar ();

		g_renderer.SetGlobalShaderConstantBuffer ("CBCamera", &cameraConstantBuffer);

		for (auto* renderer : renderers)
		{
			renderer->Render (this);
		}
	}

	void DefaultRenderLinePass::PostRender ()
	{
	}

	void DefaultRenderLinePass::BindMaterial (Material* material)
	{
		if (m_whiteMaterial == nullptr)
		{
			m_whiteMaterial = std::make_unique<Material> ();
			m_whiteMaterial->SetShader (g_assetManager.LoadShader (L"Assets/Shader/WhiteShader.hlsl"));
		}

		g_renderer.BindMaterial (m_whiteMaterial.get ());
	}

	void DefaultRenderLinePass::SetTessellation (bool bTessellation)
	{
		g_renderer.GetPlatformRenderingInterface ().SetPrimitiveTopology (EPrimitiveTopology::TriangleList);
	}
}