#pragma once

#include "Engine/Rendering/Pipeline/RenderPass.h"
#include "RI/RenderingInterfaceResource.h"

namespace GameEngine
{
	class DefaultRenderMeshPass : public RenderPass
	{
	public:
		DefaultRenderMeshPass ();
		virtual ~DefaultRenderMeshPass ();

		virtual void Release () override;

		virtual void PreRender (const RenderPipelineData& pipelineData) override;
		virtual void Render (const RenderPipelineData& pipelineData) override;
		virtual void PostRender (const RenderPipelineData& pipelineData) override;

	private:
		RenderingResourcePtr<RI_RasterizerState> m_rasterizerCullBack;
		RenderingResourcePtr<RI_DepthStencilState> m_depthLessStencilAlways;
	};
}
