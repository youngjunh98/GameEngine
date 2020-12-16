#pragma once

#include <vector>

#include "RenderPipelineData.h"

namespace GameEngine
{
	class Material;

	class RenderPass
	{
	public:
		RenderPass ();
		virtual ~RenderPass () = 0;

		virtual void Release () = 0;

		virtual void PreRender (const RenderPipelineData& pipelineData) = 0;
		virtual void Render (const RenderPipelineData& pipelineData) = 0;
		virtual void PostRender (const RenderPipelineData& pipelineData) = 0;

		virtual void BindMaterial (Material* material);
		virtual void SetTessellation (bool bTessellation);

		void UpdateLightData (const RenderPipelineData& pipelineData);
		void RenderShadowMap (const RenderPipelineData& pipelineData);
	};
}
