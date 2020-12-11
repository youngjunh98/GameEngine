#include "RenderPass.h"
#include "Engine/Rendering/GlobalRenderer.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Transform.h"
#include "Engine/Component/Camera.h"
#include "Engine/Component/Rendering/Light.h"
#include "Engine/Component/Rendering/Renderer.h"

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

	void RenderPass::UpdateLightData (const std::vector<Light*>& lights)
	{
		g_renderer.ResetLightData ();

		for (auto* light : lights)
		{
			ELightType lightType = light->GetType ();
			float range = light->GetRange ();
			float spotAngle = light->GetSpotAngle ();

			Transform& transform = light->GetGameObject ().GetTransform ();
			Vector3 position = transform.GetPosition ();
			Quaternion rotation = transform.GetRotation ();
			Vector3 forward = transform.Forward ();
			Vector3 up = transform.Up ();

			LightData data;
			data.m_color = light->GetColor ();
			data.m_worldPosition = Vector4 (position.m_x, position.m_y, position.m_z, 1.0f);
			data.m_worldDirection = Vector4 (forward.m_x, forward.m_y, forward.m_z, 0.0f);
			data.m_type = static_cast<int32> (lightType);
			data.m_intensity = light->GetIntensity ();
			data.m_range = range;
			data.m_spotAngle = spotAngle * Math::Deg2Rad;
			data.m_shadowType = static_cast<int32> (light->GetShadowType ());
			data.m_shadowIntensity = light->GetShadowIntensity ();
			data.m_shadowDepthBias = light->GetShadowDepthBias ();

			if (lightType == ELightType::Directional)
			{
				Matrix4x4 toLightSpace = Matrix4x4::LookAt (position, position + forward, up);
				Matrix4x4 toWorldSpace = toLightSpace.Inversed ();

				FrustumCorners frustum = Camera::Main->CalculateFrustumCorners ();
				AABB frustumBound;

				frustumBound.Include (toLightSpace.TransformPosition (frustum.m_nearTopLeft));
				frustumBound.Include (toLightSpace.TransformPosition (frustum.m_nearTopRight));
				frustumBound.Include (toLightSpace.TransformPosition (frustum.m_nearBottomLeft));
				frustumBound.Include (toLightSpace.TransformPosition (frustum.m_nearBottomRight));

				frustumBound.Include (toLightSpace.TransformPosition (frustum.m_farTopLeft));
				frustumBound.Include (toLightSpace.TransformPosition (frustum.m_farTopRight));
				frustumBound.Include (toLightSpace.TransformPosition (frustum.m_farBottomLeft));
				frustumBound.Include (toLightSpace.TransformPosition (frustum.m_farBottomRight));

				Vector3 boundCenter = toWorldSpace.TransformPosition (frustumBound.GetCenter ());
				Vector3 boundSize = frustumBound.GetSize ();

				Vector3 shadowCameraPosition = boundCenter - 0.5f * boundSize.m_z * forward;

				Matrix4x4 viewMatrix = Matrix4x4::LookAt (shadowCameraPosition, shadowCameraPosition + forward, up);
				Matrix4x4 projectionMatrix = Matrix4x4::Orthographic (boundSize.m_x, boundSize.m_y, 0.0001f, boundSize.m_z);

				data.m_viewProjection = (viewMatrix * projectionMatrix).Transposed ();
			}
			else if (lightType == ELightType::Point)
			{
				data.m_viewProjection = Matrix4x4::Perspective (90.0f, 1.0f, 0.0001f, range);
			}
			else if (lightType == ELightType::Spot)
			{
				Matrix4x4 viewMatrix = Matrix4x4::LookAt (position, position + forward, up);
				Matrix4x4 projectionMatrix = Matrix4x4::Perspective (spotAngle, 1.0f, 0.0001f, range);

				data.m_viewProjection = (viewMatrix * projectionMatrix).Transposed ();
			}

			g_renderer.AddLightData (data);
		}

		g_renderer.UpdateLightDataBuffer ();
	}

	void RenderPass::RenderShadowMap (const std::vector<Renderer*>& renderers)
	{
		for (int32 i = 0; i < g_renderer.GetLightCount (); i++)
		{
			LightData lightData = g_renderer.GetLightData (i);
			Vector3 lightPosition (lightData.m_worldPosition.m_x, lightData.m_worldPosition.m_y, lightData.m_worldPosition.m_z);
			ELightType lightType = static_cast<ELightType> (lightData.m_type);

			uint32 renderCount = 0;
			uint32 renderTargetOffset = 0;
			std::vector<CameraConstantBuffer> cameraBuffers;

			if (lightType == ELightType::Directional)
			{
				renderCount = 1;
				renderTargetOffset = i;

				CameraConstantBuffer cameraBuffer;
				cameraBuffer.m_viewProjection = lightData.m_viewProjection;

				cameraBuffers.push_back (cameraBuffer);
			}
			else if (lightType == ELightType::Spot)
			{
				renderCount = 1;
				renderTargetOffset = i;

				CameraConstantBuffer cameraBuffer;
				cameraBuffer.m_viewProjection = lightData.m_viewProjection;

				cameraBuffers.push_back (cameraBuffer);
			}
			else if (lightType == ELightType::Point)
			{
				renderCount = 6;
				renderTargetOffset = 6 * i;

				Matrix4x4 viewMatrices[6] = {
					Matrix4x4::LookAt (lightPosition, lightPosition + Vector3::Right, Vector3::Up),
					Matrix4x4::LookAt (lightPosition, lightPosition + Vector3::Left, Vector3::Up),

					Matrix4x4::LookAt (lightPosition, lightPosition + Vector3::Up, Vector3::Back),
					Matrix4x4::LookAt (lightPosition, lightPosition + Vector3::Down, Vector3::Forward),

					Matrix4x4::LookAt (lightPosition, lightPosition + Vector3::Forward, Vector3::Up),
					Matrix4x4::LookAt (lightPosition, lightPosition + Vector3::Back, Vector3::Up)
				};

				for (int32 renderTargetIndex = 0; renderTargetIndex < 6; renderTargetIndex++)
				{
					CameraConstantBuffer cameraBuffer;
					cameraBuffer.m_cameraWorldPosition = lightPosition;
					cameraBuffer.m_viewProjection = (viewMatrices[renderTargetIndex] * lightData.m_viewProjection).Transposed ();
					cameraBuffer.m_cameraFar = lightData.m_range;

					cameraBuffers.push_back (cameraBuffer);
				}
			}

			g_renderer.ActivateShadowMapShader (lightType);

			for (uint32 i = 0; i < renderCount; i++)
			{
				g_renderer.BindShadowRenderTarget (lightType, renderTargetOffset + i);
				g_renderer.SetGlobalShaderConstantBuffer ("CBCamera", &cameraBuffers.at (i));

				for (auto* renderer : renderers)
				{
					renderer->Render (this);
				}
			}
		}
	}
}