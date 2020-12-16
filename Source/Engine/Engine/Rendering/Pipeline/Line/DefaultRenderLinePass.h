#pragma once

#include <memory>

#include "Engine/Engine/Rendering/Pipeline/RenderPass.h"
#include "Engine/Engine/Rendering/Material.h"
#include "Engine/RI/RenderingInterfaceResource.h"

namespace GameEngine
{
	class DefaultRenderLinePass : public RenderPass
	{
	public:
		DefaultRenderLinePass ();
		virtual ~DefaultRenderLinePass ();

		virtual void Release () override;

		virtual void PreRender (const RenderPipelineData& pipelineData) override;
		virtual void Render (const RenderPipelineData& pipelineData) override;
		virtual void PostRender (const RenderPipelineData& pipelineData) override;

		virtual void BindMaterial (Material* material) override;
		virtual void SetTessellation (bool bTessellation) override;

	private:
		RenderingResourcePtr<RI_RasterizerState> m_rsLineCullNone;
		RenderingResourcePtr<RI_DepthStencilState> m_depthLessStencilAlways;
		
		std::unique_ptr<Material> m_whiteMaterial;
	};
}