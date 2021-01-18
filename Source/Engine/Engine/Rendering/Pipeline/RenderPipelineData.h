#pragma once

#include <vector>

#include "Type.h"
#include "Engine/Core/Math/Matrix4x4.h"
#include "Engine/Core/Math/Vector3.h"
#include "Engine/Engine/Component/Camera.h"

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
			float m_fov;
			float m_aspectRatio;
			FrustumCorners m_frustum;
		};

		CameraData m_camera;
		std::vector<Renderer*> m_renderers;
		std::vector<Light*> m_lights;
	};
}
