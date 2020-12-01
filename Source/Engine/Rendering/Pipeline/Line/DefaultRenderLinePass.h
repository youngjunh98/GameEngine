#pragma once

#include <memory>

#include "Engine/Rendering/Pipeline/RenderPass.h"
#include "Engine/Rendering/Material.h"
#include "RI/RenderingInterfaceResource.h"

namespace GameEngine
{
	class DefaultRenderLinePass : public RenderPass
	{
	public:
		DefaultRenderLinePass () {}
		virtual ~DefaultRenderLinePass () {}

		virtual void Release () override;

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