#pragma once

#include "Rendering/Pipeline/RenderPipeline.h"
#include "Rendering/Pipeline/Line/DefaultRenderLinePass.h"

namespace GameEngine
{
	class DefaultLineRenderPipeline : public RenderPipeline
	{
	public:
		DefaultLineRenderPipeline () {}
		virtual ~DefaultLineRenderPipeline () {}

		void Start (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*>& lights) override;
		void End () override;

	private:
		DefaultRenderLinePass m_lineRenderPass;
	};
}