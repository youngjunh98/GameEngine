#pragma once

#include "Engine/Rendering/Pipeline/RenderPass.h"
#include "RI/RenderingInterfaceResource.h"

namespace GameEngine
{
	class DefaultShadowPass : public RenderPass
	{
	public:
		DefaultShadowPass ();
		virtual ~DefaultShadowPass ();

		virtual void Release () override;

		virtual void PreRender (const RenderPipelineData& pipelineData) override;
		virtual void Render (const RenderPipelineData& pipelineData) override;
		virtual void PostRender (const RenderPipelineData& pipelineData) override;

		virtual void BindMaterial (Material* material) override;
		virtual void SetTessellation (bool bTessellation) override;
	};
}
