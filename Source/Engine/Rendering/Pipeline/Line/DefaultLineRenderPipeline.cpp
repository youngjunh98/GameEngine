#include "DefaultLineRenderPipeline.h"

namespace GameEngine
{
	void DefaultLineRenderPipeline::Release ()
	{
		m_lineRenderPass.Release ();
	}

	void DefaultLineRenderPipeline::Start (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*>& lights)
	{
		m_lineRenderPass.PreRender (cameras, renderers, lights);
		m_lineRenderPass.Render (cameras, renderers, lights);
		m_lineRenderPass.PostRender ();
	}

	void DefaultLineRenderPipeline::End ()
	{
	}
}
