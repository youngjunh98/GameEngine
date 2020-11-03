#include "Rendering/Pipeline/RenderPass.h"
#include "Rendering/GlobalRenderer.h"

namespace GameEngine
{
	void RenderPass::BindMaterial (Material* material)
	{
		g_renderer.BindMaterial (material);
	}

	void RenderPass::SetTessellation (bool bTessellation)
	{
		g_renderer.GetPlatformRenderingInterface ().SetPrimitiveTopology (bTessellation ? EPrimitiveTopology::TrianglePatchList : EPrimitiveTopology::TriangleList);
	}
}