#include "RenderPass.h"
#include "Engine/Rendering/GlobalRenderer.h"

namespace GameEngine
{
	void RenderPass::BindMaterial (Material* material)
	{
		g_renderer.BindMaterial (material);
	}

	void RenderPass::SetTessellation (bool bTessellation)
	{
		g_renderer.GetRenderingInterface ().SetPrimitiveTopology (bTessellation ? EPrimitiveTopology::TrianglePatchList : EPrimitiveTopology::TriangleList);
	}
}