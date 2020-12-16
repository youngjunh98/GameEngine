#include "DefaultRenderMeshPass.h"
#include "Engine/RI/RenderingInterface.h"
#include "Engine/Engine/Rendering/GlobalRenderer.h"
#include "Engine/Engine/GameObject.h"
#include "Engine/Engine/Component/Transform.h"
#include "Engine/Engine/Component/Camera.h"
#include "Engine/Engine/Component/Rendering/Renderer.h"

namespace GameEngine
{
	DefaultRenderMeshPass::DefaultRenderMeshPass ()
	{
	}

	DefaultRenderMeshPass::~DefaultRenderMeshPass ()
	{
	}

	void DefaultRenderMeshPass::Release ()
	{
		m_rasterizerCullBack = nullptr;
		m_depthLessStencilAlways = nullptr;
	}

	void DefaultRenderMeshPass::PreRender (const RenderPipelineData& pipelineData)
	{
		RenderingInterface& renderingInterface = RenderingInterface::GetModule ();

		if (m_rasterizerCullBack.get () == nullptr)
		{
			m_rasterizerCullBack = renderingInterface.CreateRasterizerState (EFillMode::Solid, EFaceCulling::Back, EWindingOrder::CW);
		}

		if (m_depthLessStencilAlways.get () == nullptr)
		{
			m_depthLessStencilAlways = renderingInterface.CreateDepthStencilState (EComparisonFunction::LessEqual, EComparisonFunction::Always);
		}

		renderingInterface.SetRasterizerState (m_rasterizerCullBack.get ());
		renderingInterface.SetDepthStencilState (m_depthLessStencilAlways.get ());

		Vector2 renderSize = GlobalRenderer::GetRenderSize ();
		GlobalRenderer::SetViewport (renderSize);

		GlobalRenderer::BindRenderTargetAndDepthStencil ();
		GlobalRenderer::ClearRenderTargetAndDepthStencil (Vector4 (0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);
	}

	void DefaultRenderMeshPass::Render (const RenderPipelineData& pipelineData)
	{
		CameraConstantBuffer cameraConstantBuffer;
		cameraConstantBuffer.m_view = pipelineData.m_camera.m_viewMatrix.Transposed ();
		cameraConstantBuffer.m_projection = pipelineData.m_camera.m_projectionMatrix.Transposed ();
		cameraConstantBuffer.m_viewProjection = cameraConstantBuffer.m_projection * cameraConstantBuffer.m_view;
		cameraConstantBuffer.m_cameraWorldPosition = pipelineData.m_camera.m_position;
		cameraConstantBuffer.m_cameraFar = pipelineData.m_camera.m_far;

		GlobalRenderer::SetGlobalShaderConstantBuffer ("CBCamera", &cameraConstantBuffer);

		for (auto* renderer : pipelineData.m_renderers)
		{
			renderer->Render (this);
		}
	}

	void DefaultRenderMeshPass::PostRender (const RenderPipelineData& pipelineData)
	{
	}
}
