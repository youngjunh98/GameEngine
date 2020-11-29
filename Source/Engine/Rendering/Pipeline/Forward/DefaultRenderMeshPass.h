#ifndef INCLUDE_DEFAULT_RENDER_MESH_PASS
#define INCLUDE_DEFAULT_RENDER_MESH_PASS

#include "Engine/Rendering/Pipeline/RenderPass.h"
#include "RI/RenderingInterfaceResource.h"

namespace GameEngine
{
	class DefaultRenderMeshPass : public RenderPass
	{
	public:
		DefaultRenderMeshPass () {}
		virtual ~DefaultRenderMeshPass () {}

		virtual void Release () override;

		virtual void PreRender (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights) override;
		virtual void Render (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights) override;
		virtual void PostRender () override;

	private:
		RenderingResourcePtr<RI_RasterizerState> m_rasterizerCullBack;
		RenderingResourcePtr<RI_DepthStencilState> m_depthLessStencilAlways;
	};
}

#endif