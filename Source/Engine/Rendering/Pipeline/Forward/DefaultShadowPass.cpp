#include "DefaultShadowPass.h"
#include "Platform/PlatformRenderingInterface.h"
#include "Engine/Rendering/GlobalRenderer.h"

namespace GameEngine
{
	void DefaultShadowPass::Release ()
	{
	}

	void DefaultShadowPass::PreRender (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights)
	{
		UpdateLightData (lights);
	}

	void DefaultShadowPass::Render (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights)
	{
		RenderShadowMap (renderers);
	}

	void DefaultShadowPass::PostRender ()
	{
	}

	void DefaultShadowPass::BindMaterial (Material* material)
	{
	}

	void DefaultShadowPass::SetTessellation (bool bTessellation)
	{
	}
}
