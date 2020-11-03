#include "Rendering/Pipeline/RenderPipeline.h"

namespace GameEngine
{
	void RenderPipeline::AddPass (RenderPass* renderPass)
	{
		m_renderPasses.push_back (renderPass);
	}
}