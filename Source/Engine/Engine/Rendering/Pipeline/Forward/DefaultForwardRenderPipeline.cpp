#include "DefaultForwardRenderPipeline.h"
#include "Engine/Engine/Rendering/GlobalRenderer.h"

namespace GameEngine
{
	DefaultForwardRenderPipeline::DefaultForwardRenderPipeline ()
	{
	}

	DefaultForwardRenderPipeline::~DefaultForwardRenderPipeline()
	{
	}

	void DefaultForwardRenderPipeline::Release ()
	{
		m_shadowPass.Release ();
		m_meshRenderPass.Release ();
	}

	void DefaultForwardRenderPipeline::Execute (const RenderPipelineData& pipelineData)
	{
		m_shadowPass.PreRender (pipelineData);
		m_shadowPass.Render (pipelineData);
		m_shadowPass.PostRender (pipelineData);

		m_meshRenderPass.PreRender (pipelineData);
		m_meshRenderPass.Render (pipelineData);
		m_meshRenderPass.PostRender (pipelineData);

		GlobalRenderer::DrawSkybox (pipelineData.m_camera.m_viewMatrix, pipelineData.m_camera.m_near, pipelineData.m_camera.m_far, pipelineData.m_camera.m_fov, pipelineData.m_camera.m_aspectRatio);
	}
}
