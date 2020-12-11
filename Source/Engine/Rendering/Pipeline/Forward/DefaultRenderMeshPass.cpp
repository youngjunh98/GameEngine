#include "DefaultRenderMeshPass.h"
#include "Engine/Rendering/GlobalRenderer.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Transform.h"
#include "Engine/Component/Camera.h"
#include "Engine/Component/Rendering/Renderer.h"

namespace GameEngine
{
	void DefaultRenderMeshPass::Release ()
	{
		m_rasterizerCullBack = nullptr;
		m_depthLessStencilAlways = nullptr;
	}

	void DefaultRenderMeshPass::PreRender (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights)
	{
		if (m_rasterizerCullBack.get () == nullptr)
		{
			m_rasterizerCullBack = GlobalRenderer::GetRenderingInterface ().CreateRasterizerState (EFillMode::Solid, EFaceCulling::Back, EWindingOrder::CW);
		}

		if (m_depthLessStencilAlways.get () == nullptr)
		{
			m_depthLessStencilAlways = GlobalRenderer::GetRenderingInterface ().CreateDepthStencilState (EComparisonFunction::LessEqual, EComparisonFunction::Always);
		}

		GlobalRenderer::GetRenderingInterface ().SetRasterizerState (m_rasterizerCullBack.get ());
		GlobalRenderer::GetRenderingInterface ().SetDepthStencilState (m_depthLessStencilAlways.get ());

		Vector2 renderSize = GlobalRenderer::GetRenderSize ();
		GlobalRenderer::SetViewport (renderSize);

		GlobalRenderer::BindRenderTargetAndDepthStencil ();
		GlobalRenderer::ClearRenderTargetAndDepthStencil (Vector4 (0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);
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

		GlobalRenderer::SetGlobalShaderConstantBuffer ("CBCamera", &cameraConstantBuffer);

		for (auto* renderer : renderers)
		{
			renderer->Render (this);
		}
	}

	void DefaultRenderMeshPass::PostRender ()
	{
		
	}
}
