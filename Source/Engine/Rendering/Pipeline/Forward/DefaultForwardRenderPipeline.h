#ifndef INCLUDE_DEFAULT_FORWARD_RENDER_PIPELINE
#define INCLUDE_DEFAULT_FORWARD_RENDER_PIPELINE

#include "Engine/Rendering/Pipeline/RenderPipeline.h"
#include "Engine/Rendering/Pipeline/Forward/DefaultRenderMeshPass.h"
#include "Engine/Rendering/Pipeline/Forward/DefaultShadowPass.h"

namespace GameEngine
{
	class DefaultForwardRenderPipeline : public RenderPipeline
	{
	public:
		DefaultForwardRenderPipeline () {}
		virtual ~DefaultForwardRenderPipeline () {}

		virtual void Release () override;

		void Start (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*>& lights) override;
		void End () override;

	private:
		DefaultShadowPass m_shadowPass;
		DefaultRenderMeshPass m_meshRenderPass;
	};
}

#endif