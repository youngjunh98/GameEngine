#pragma once

#include <vector>

#include "Type.h"
#include "Core/Math/Matrix4x4.h"
#include "Core/Math/Vector3.h"

namespace GameEngine
{
    class Renderer;
    class Light;

    struct RenderPipelineData
	{
		struct CameraData
		{
			Matrix4x4 m_viewMatrix;
			Matrix4x4 m_projectionMatrix;
            Vector3 m_position;
			float m_near;
			float m_far;
		};

		CameraData m_camera;
		std::vector<Renderer*> m_renderers;
		std::vector<Light*> m_lights;
	};
}
