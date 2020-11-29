#pragma once

#include "Engine/Rendering/Pipeline/RenderPipeline.h"
#include "Engine/Rendering/Pipeline/Line/DefaultRenderLinePass.h"

namespace GameEngine
{
	class DefaultLineRenderPipeline : public RenderPipeline
	{
	public:
		DefaultLineRenderPipeline () {}
		virtual ~DefaultLineRenderPipeline () {}

		virtual void Release () override;

		void Start (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*>& lights) override;
		void End () override;

	private:
		DefaultRenderLinePass m_lineRenderPass;
	};
}