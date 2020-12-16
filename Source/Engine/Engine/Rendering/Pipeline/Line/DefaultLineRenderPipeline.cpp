#include "DefaultLineRenderPipeline.h"

namespace GameEngine
{
	DefaultLineRenderPipeline::DefaultLineRenderPipeline ()
	{
	}

	DefaultLineRenderPipeline::~DefaultLineRenderPipeline ()
	{
	}

	void DefaultLineRenderPipeline::Release ()
	{
		m_lineRenderPass.Release ();
	}

	void DefaultLineRenderPipeline::Execute (const RenderPipelineData& pipelineData)
	{
		m_lineRenderPass.PreRender (pipelineData);
		m_lineRenderPass.Render (pipelineData);
		m_lineRenderPass.PostRender (pipelineData);
	}
}
