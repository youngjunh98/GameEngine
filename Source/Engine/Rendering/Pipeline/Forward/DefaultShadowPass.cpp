#include "DefaultShadowPass.h"
#include "Platform/PlatformRenderingInterface.h"
#include "Engine/Rendering/GlobalRenderer.h"

namespace GameEngine
{
	DefaultShadowPass::DefaultShadowPass ()
	{
	}

	DefaultShadowPass::~DefaultShadowPass ()
	{
	}

	void DefaultShadowPass::Release ()
	{
	}

	void DefaultShadowPass::PreRender (const RenderPipelineData& pipelineData)
	{
		UpdateLightData (pipelineData);
	}

	void DefaultShadowPass::Render (const RenderPipelineData& pipelineData)
	{
		RenderShadowMap (pipelineData);
	}

	void DefaultShadowPass::PostRender (const RenderPipelineData& pipelineData)
	{
	}

	void DefaultShadowPass::BindMaterial (Material* material)
	{
	}

	void DefaultShadowPass::SetTessellation (bool bTessellation)
	{
	}
}
