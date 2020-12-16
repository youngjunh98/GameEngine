#pragma once

#include <vector>

#include "RenderPipelineData.h"

namespace GameEngine
{
	class RenderPipeline
	{
	public:
		RenderPipeline ();
		virtual ~RenderPipeline () = 0;

		virtual void Release () = 0;
		virtual void Execute (const RenderPipelineData& pipelineData) = 0;
	};
}
