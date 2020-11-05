#ifndef INCLUDE_DEFAULT_SHADOW_PASS
#define INCLUDE_DEFAULT_SHADOW_PASS

#include "Rendering/Pipeline/RenderPass.h"
#include "RI/RenderingInterfaceResource.h"

namespace GameEngine
{
	class DefaultShadowPass : public RenderPass
	{
	public:
		DefaultShadowPass () {}
		virtual ~DefaultShadowPass () {}

		virtual void Release () override;

		virtual void PreRender (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights) override;
		virtual void Render (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights) override;
		virtual void PostRender () override;

		virtual void BindMaterial (Material* material) override;
		virtual void SetTessellation (bool bTessellation) override;
	};
}

#endif