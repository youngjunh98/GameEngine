#include "Rendering/Pipeline/Forward/DefaultForwardRenderPipeline.h"
#include "Rendering/GlobalRenderer.h"

namespace GameEngine
{
	void DefaultForwardRenderPipeline::Release ()
	{
		m_shadowPass.Release ();
		m_meshRenderPass.Release ();
	}

	void DefaultForwardRenderPipeline::Start (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*>& lights)
	{
		m_shadowPass.PreRender (cameras, renderers, lights);
		m_shadowPass.Render (cameras, renderers, lights);
		m_shadowPass.PostRender ();

		m_meshRenderPass.PreRender (cameras, renderers, lights);
		m_meshRenderPass.Render (cameras, renderers, lights);
		m_meshRenderPass.PostRender ();
	}

	void DefaultForwardRenderPipeline::End ()
	{
		g_renderer.DrawSkybox ();
	}
}
