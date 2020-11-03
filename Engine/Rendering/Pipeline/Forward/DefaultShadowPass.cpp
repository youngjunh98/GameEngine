#include "Rendering/Pipeline/Forward/DefaultShadowPass.h"
#include "Rendering/GlobalRenderer.h"
#include "Platform/PlatformRenderingInterface.h"
#include "GameObject.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
#include "Component/Rendering/Light.h"
#include "Component/Rendering/Renderer.h"

namespace GameEngine
{
	void DefaultShadowPass::PreRender (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights)
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

				FrustumCorners frustum;
				AABB frustumBound;

				if (cameras.size () > 0)
				{
					frustum = cameras.at (0)->CalculateFrustumCorners ();
				}

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

	void DefaultShadowPass::Render (const std::vector<Camera*>& cameras, const std::vector<Renderer*>& renderers, const std::vector<Light*> lights)
	{
		for (int32 i = 0; i < g_renderer.GetLightCount (); i++)
		{
			LightData lightData = g_renderer.GetLightData (i);
			ELightType lightType = static_cast<ELightType> (lightData.m_type);

			g_renderer.ActivateShadowMapShader (lightData.m_type);

			if (lightType == ELightType::Directional)
			{
				g_renderer.BindShadowRenderTarget (lightData.m_type, i);

				CameraConstantBuffer cameraBuffer;
				cameraBuffer.m_viewProjection = lightData.m_viewProjection;

				g_renderer.SetGlobalShaderConstantBuffer ("CBCamera", &cameraBuffer);

				for (auto* renderer : renderers)
				{
					renderer->Render (this);
				}
			}
			else if (lightType == ELightType::Point)
			{
				Vector3 lightPosition (lightData.m_worldPosition.m_x, lightData.m_worldPosition.m_y, lightData.m_worldPosition.m_z);

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
					g_renderer.BindShadowRenderTarget (lightData.m_type, 6 * i + renderTargetIndex);

					CameraConstantBuffer cameraBuffer;
					cameraBuffer.m_cameraWorldPosition = Vector3 (lightData.m_worldPosition.m_x, lightData.m_worldPosition.m_y, lightData.m_worldPosition.m_z);
					cameraBuffer.m_viewProjection = (viewMatrices[renderTargetIndex] * lightData.m_viewProjection).Transposed ();
					cameraBuffer.m_cameraFar = lightData.m_range;

					g_renderer.SetGlobalShaderConstantBuffer ("CBCamera", &cameraBuffer);

					for (auto* renderer : renderers)
					{
						renderer->Render (this);
					}
				}
			}
			else if (lightType == ELightType::Spot)
			{
				g_renderer.BindShadowRenderTarget (lightData.m_type, i);

				CameraConstantBuffer cameraBuffer;
				cameraBuffer.m_viewProjection = lightData.m_viewProjection;

				g_renderer.SetGlobalShaderConstantBuffer ("CBCamera", &cameraBuffer);

				for (auto* renderer : renderers)
				{
					renderer->Render (this);
				}
			}
		}
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
