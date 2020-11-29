#ifndef INCLUDE_RENDER_PIPELINE
#define INCLUDE_RENDER_PIPELINE

#include <vector>

#include "Core/CoreMinimal.h"
#include "Engine/Rendering/Pipeline/RenderPass.h"

namespace GameEngine
{
	struct RenderPipelineData
	{
		std::vector<Camera*> m_cameras;
		std::vector<Renderer*> m_renderers;
		std::vector<Light*> m_data;
	};

	struct CullData
	{
		Camera* m_camera;
		std::vector<Renderer*> m_visibleRenderers;
	};

	class RenderPipeline
	{
	public:
		RenderPipeline () {}
		virtual ~RenderPipeline () = 0 {};

		virtual void Release () = 0;

		virtual void Start (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*>& lights) = 0;
		virtual void End () = 0;

		void AddPass (RenderPass* renderPass);

	protected:
		std::vector<RenderPass*> m_renderPasses;
	};
}

#endif