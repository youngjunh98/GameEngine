#include "Rendering/Pipeline/Forward/DefaultRenderMeshPass.h"
#include "Rendering/GlobalRenderer.h"
#include "GameObject.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
#include "Component/Rendering/Light.h"
#include "Component/Rendering/Renderer.h"

namespace GameEngine
{
	void DefaultRenderMeshPass::PreRender (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights)
	{
		if (m_rasterizerCullBack.get () == nullptr)
		{
			m_rasterizerCullBack = g_renderer.GetPlatformRenderingInterface ().CreateRasterizerState (EFillMode::Solid, EFaceCulling::Back, EWindingOrder::CW);
		}

		if (m_depthLessStencilAlways.get () == nullptr)
		{
			m_depthLessStencilAlways = g_renderer.GetPlatformRenderingInterface ().CreateDepthStencilState (EComparisonFunction::LessEqual, EComparisonFunction::Always);
		}

		g_renderer.GetPlatformRenderingInterface ().SetRasterizerState (m_rasterizerCullBack.get ());
		g_renderer.GetPlatformRenderingInterface ().SetDepthStencilState (m_depthLessStencilAlways.get ());

		Vector2 screenRenderSize = g_renderer.GetScreenRenderSize ();

		g_renderer.BindRenderTarget (nullptr, nullptr);
		g_renderer.ClearRenderTarget (nullptr, nullptr, Vector4 (0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

		g_renderer.GetPlatformRenderingInterface ().SetViewport (static_cast<uint32> (screenRenderSize.m_x), static_cast<uint32> (screenRenderSize.m_y), 0, 0);
	}

	void DefaultRenderMeshPass::Render (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights)
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

	void DefaultRenderMeshPass::PostRender ()
	{
		
	}
}
