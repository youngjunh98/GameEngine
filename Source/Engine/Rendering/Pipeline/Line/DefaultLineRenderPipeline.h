#pragma once

#include "Engine/Rendering/Pipeline/RenderPipeline.h"
#include "Engine/Rendering/Pipeline/Line/DefaultRenderLinePass.h"

namespace GameEngine
{
	class DefaultLineRenderPipeline : public RenderPipeline
	{
	public:
		DefaultLineRenderPipeline ();
		virtual ~DefaultLineRenderPipeline ();

		virtual void Release () override;
		virtual void Execute (const RenderPipelineData& pipelineData) override;

	private:
		DefaultRenderLinePass m_lineRenderPass;
	};
}