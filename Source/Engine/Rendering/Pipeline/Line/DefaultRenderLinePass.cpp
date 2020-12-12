#include "DefaultRenderLinePass.h"
#include "Platform/PlatformRenderingInterface.h"
#include "Engine/Rendering/GlobalRenderer.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Transform.h"
#include "Engine/Component/Camera.h"
#include "Engine/Component/Rendering/Renderer.h"
#include "Engine/Asset/AssetManager.h"

namespace GameEngine
{
	DefaultRenderLinePass::DefaultRenderLinePass ()
	{
	}

	DefaultRenderLinePass::~DefaultRenderLinePass()
	{
	}

	void DefaultRenderLinePass::Release ()
	{
		m_rsLineCullNone = nullptr;
		m_depthLessStencilAlways = nullptr;
		m_whiteMaterial = nullptr;
	}

	void DefaultRenderLinePass::PreRender (const RenderPipelineData& pipelineData)
	{
		if (m_rsLineCullNone.get () == nullptr)
		{
			m_rsLineCullNone = GlobalRenderer::GetRenderingInterface ().CreateRasterizerState (EFillMode::Wireframe, EFaceCulling::None, EWindingOrder::CW);
		}

		if (m_depthLessStencilAlways.get () == nullptr)
		{
			m_depthLessStencilAlways = GlobalRenderer::GetRenderingInterface ().CreateDepthStencilState (EComparisonFunction::Always, EComparisonFunction::Always);
		}

		GlobalRenderer::GetRenderingInterface ().SetRasterizerState (m_rsLineCullNone.get ());
		GlobalRenderer::GetRenderingInterface ().SetDepthStencilState (m_depthLessStencilAlways.get ());

		Vector2 renderSize = GlobalRenderer::GetRenderSize ();
		GlobalRenderer::SetViewport (renderSize);

		GlobalRenderer::BindRenderTargetAndDepthStencil ();
		GlobalRenderer::ClearRenderTargetAndDepthStencil (Vector4 (0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);
	}

	void DefaultRenderLinePass::Render (const RenderPipelineData& pipelineData)
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

	void DefaultRenderLinePass::PostRender (const RenderPipelineData& pipelineData)
	{
	}

	void DefaultRenderLinePass::BindMaterial (Material* material)
	{
		if (m_whiteMaterial == nullptr)
		{
			m_whiteMaterial = std::make_unique<Material> ();
			m_whiteMaterial->SetShader (g_assetManager.LoadShader (L"Assets/Shader/WhiteShader.hlsl"));
		}

		GlobalRenderer::BindMaterial (m_whiteMaterial.get ());
	}

	void DefaultRenderLinePass::SetTessellation (bool bTessellation)
	{
		GlobalRenderer::GetRenderingInterface ().SetPrimitiveTopology (EPrimitiveTopology::TriangleList);
	}
}