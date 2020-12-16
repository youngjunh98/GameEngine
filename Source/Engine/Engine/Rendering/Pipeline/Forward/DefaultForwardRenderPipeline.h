#pragma once

#include "Engine/Engine/Rendering/Pipeline/RenderPipeline.h"
#include "Engine/Engine/Rendering/Pipeline/Forward/DefaultRenderMeshPass.h"
#include "Engine/Engine/Rendering/Pipeline/Forward/DefaultShadowPass.h"

namespace GameEngine
{
	class DefaultForwardRenderPipeline : public RenderPipeline
	{
	public:
		DefaultForwardRenderPipeline ();
		virtual ~DefaultForwardRenderPipeline ();

		virtual void Release () override;
		virtual void Execute (const RenderPipelineData& pipelineData) override;

	private:
		DefaultShadowPass m_shadowPass;
		DefaultRenderMeshPass m_meshRenderPass;
	};
}
