#pragma once

#include <memory>

#include "Rendering/Pipeline/RenderPass.h"
#include "RI/RenderingInterfaceResource.h"
#include "Rendering/Material.h"

namespace GameEngine
{
	class DefaultRenderLinePass : public RenderPass
	{
	public:
		DefaultRenderLinePass () {}
		virtual ~DefaultRenderLinePass () {}

		virtual void PreRender (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights) override;
		virtual void Render (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights) override;
		virtual void PostRender () override;

		virtual void BindMaterial (Material* material) override;
		virtual void SetTessellation (bool bTessellation) override;

	private:
		RenderingResourcePtr<RI_RasterizerState> m_rsLineCullNone;
		RenderingResourcePtr<RI_DepthStencilState> m_depthLessStencilAlways;
		
		std::unique_ptr<Material> m_whiteMaterial;
	};
}