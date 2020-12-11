#ifndef INCLUDE_RENDER_PASS
#define INCLUDE_RENDER_PASS

#include <vector>

#include "Core/CoreMinimal.h"

namespace GameEngine
{
	class RenderPass
	{
	public:
		RenderPass () {}
		virtual ~RenderPass () = 0 {}

		virtual void Release () = 0;

		virtual void PreRender (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights) = 0;
		virtual void Render (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights) = 0;
		virtual void PostRender () = 0;

		virtual void BindMaterial (Material* material);
		virtual void SetTessellation (bool bTessellation);

		void UpdateLightData (const std::vector<Light*>& lights);
		void RenderShadowMap (const std::vector<Renderer*>& renderers);
	};
}

#endif