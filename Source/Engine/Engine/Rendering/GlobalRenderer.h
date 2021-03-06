#pragma once

#include <vector>
#include <memory>

#include "Type.h"
#include "Engine/Core/CoreMinimal.h"
#include "Engine/RI/RenderingInterface.h"
#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Rendering/Shader.h"
#include "Engine/Engine/Rendering/Mesh.h"
#include "Engine/Engine/Rendering/TextureCube.h"
#include "Engine/Engine/Rendering/Material.h"
#include "Engine/Engine/Rendering/Pipeline/RenderPipeline.h"
#include "Engine/Engine/Rendering/Pipeline/Forward/DefaultForwardRenderPipeline.h"
#include "Engine/Engine/Rendering/Pipeline/Line/DefaultLineRenderPipeline.h"
#include "Engine/Engine/Rendering/GlobalRendererSettings.h"
#include "Engine/Engine/Component/Rendering/Light.h"

namespace GameEngine
{
	struct ObjectConstantBuffer
	{
		Matrix4x4 m_localToWorld;
		Matrix4x4 m_worldToLocal;
	};

	struct CameraConstantBuffer
	{
		Matrix4x4 m_view;
		Matrix4x4 m_projection;
		Matrix4x4 m_viewProjection;
		Vector3 m_cameraWorldPosition;
		float m_cameraFar;
	};

	struct LightData
	{
		Vector4 m_color;
		Vector4 m_worldPosition;
		Vector4 m_worldDirection;
		int32 m_type;
		float m_intensity;
		float m_range;
		float m_spotAngle;
		Matrix4x4 m_viewProjection;
		int32 m_shadowType;
		float m_shadowIntensity;
		float m_shadowDepthBias;
		int32 m_padding;
	};

	struct LightingConstantBuffer
	{
		int32 m_lightCount;
		float m_shadowDistance;
		int64 m_padding;
		LightData m_data[3];
	};

	class ENGINE_API GlobalRenderer final
	{
	private:
		GlobalRenderer ();
		~GlobalRenderer ();

		static GlobalRenderer& GetInstance ();

	public:
		GlobalRenderer (const GlobalRenderer&) = delete;
		GlobalRenderer& operator= (const GlobalRenderer&) = delete;

		static bool Init (const GlobalRendererSettings settings);
		static void Shutdown ();

		static void RenderScene (Scene* scene);
		static void RenderScene (Scene* scene, RenderPipeline& pipeline, Matrix4x4 viewMatrix, Matrix4x4 projectionMatrix, float aspectRatio,
			Vector3 cameraPosition, float cameraFov, float cameraNear, float cameraFar);

		static void PresentSwapChain ();
		static bool ResizeSwapChain (uint32 width, uint32 height, bool bFullscreen);
		static bool UpdateSwapChainResource ();
		static RI_RenderTargetView* GetSwapChainRenderTarget ();
		static uint32 GetSwapChainWidth ();
		static uint32 GetSwapChainHeight ();

		static void ChangeScreenSize (uint32 width, uint32 height, bool bFullscreen);
		static bool UpdateScreenDepthStencilResource ();
		static RI_DepthStencilView* GetScreenDepthStencil ();

		static void BindRenderTargetAndDepthStencil ();
		static void ClearRenderTargetAndDepthStencil (Vector4 color, float depth, uint8 stencil);
		static RenderingResourcePtr<RI_RenderTargetView> GetRenderTarget ();
		static RenderingResourcePtr<RI_DepthStencilView> GetDepthStencil ();
		static void SetRenderTargetAndDepthStencilAsDefault ();
		static void SetRenderTarget (RenderingResourcePtr<RI_RenderTargetView> renderTarget);
		static void SetDepthStencil (RenderingResourcePtr<RI_DepthStencilView> depthStencil);

		static Vector2 GetRenderSize ();
		static void SetRenderSize (Vector2 size);

		static Vector2 GetViewportSize ();
		static Vector2 GetViewportTopLeft ();
		static void SetViewport (Vector2 size, Vector2 topLeft = Vector2::Zero);

		static void ActivateShadowMapShader (ELightType lightType);
		static void BindShadowRenderTarget (ELightType lightType, uint32 renderTargetIndex);

		static GlobalRendererSettings GetSettings ();

		static void BindRenderPipeline (RenderPipeline* renedrPipeline);
		static DefaultForwardRenderPipeline* GetDefaultForwardRenderPipeline ();
		static DefaultLineRenderPipeline* GetDefaultLineRenderPipeline ();

		static void BindMaterial (Material* material);
		static void BindGlobalShaderConstantBuffer (Shader* shader, const std::string& name);
		static void SetGlobalShaderConstantBuffer (const std::string& name, const void* bufferData);

		static void DrawVertices (RI_VertexBuffer* vertexBuffer, RI_IndexBuffer* indexBuffer);

		static void DrawSkybox (Matrix4x4 viewMatrix, float cameraNear, float cameraFar, float cameraFov, float aspectRatio);

		static LightData GetLightData (uint32 index);
		static void AddLightData (LightData data);
		static void ResetLightData ();
		static void UpdateLightDataBuffer ();

		static int32 GetLightCount ();
		static int32 GetMaxLightCount ();

		static std::shared_ptr<Shader>& GetStandardShader ();

	private:
		static void LoadInternalRenderingAsset ();
		static void UnloadInternalRenderingAsset ();

	private:
		GlobalRendererSettings m_settings;
		RenderingInterface* m_ri;

		RenderingResourcePtr<RI_Texture2D> m_swapChainBuffer;
		RenderingResourcePtr<RI_RenderTargetView> m_swapChainRenderTarget;
		RenderingResourcePtr<RI_Texture2D> m_screenDepthStencilBuffer;
		RenderingResourcePtr<RI_DepthStencilView> m_screenDepthStencil;

		RenderingResourcePtr<RI_RenderTargetView> m_usingRenderTarget;
		RenderingResourcePtr<RI_DepthStencilView> m_usingDepthStencil;

		Vector2 m_viewportSize;
		Vector2 m_viewportTopLeft;

		int32 m_maxLightCount;
		std::vector<LightData> m_lightData;
		LightingConstantBuffer m_lightBuffer;

		RenderPipeline* m_renderPipeline;
		DefaultForwardRenderPipeline m_defaultForwardPipeline;
		DefaultLineRenderPipeline m_defaultLinePipeline;

		RenderingResourcePtr<RI_RasterizerState> m_rsCullBack;
		RenderingResourcePtr<RI_RasterizerState> m_rsCullNone;

		RenderingResourcePtr<RI_DepthStencilState> m_depthLessStencilAlways;
		RenderingResourcePtr<RI_DepthStencilState> m_dssLessEqualStencilAlways;

		RenderingResourcePtr<RI_ShaderConstantBuffer> m_constantBufferPerObject;
		RenderingResourcePtr<RI_ShaderConstantBuffer> m_constantBufferPerCamera;
		RenderingResourcePtr<RI_ShaderConstantBuffer> m_lightingConstantBuffer;

		RenderingResourcePtr<RI_Sampler> m_shadowMapSampler;

		RenderingResourcePtr<RI_Texture2D> m_directionalLightShadowMap;
		RenderingResourcePtr<RI_ShaderResourceView> m_directionalLightShadowMapSRV;
		std::vector<RenderingResourcePtr<RI_DepthStencilView>> m_directionalLightShadowMapDSV;
		uint32 m_directionalLightShadowMapLastSlot;

		RenderingResourcePtr<RI_Texture2D> m_pointLightShadowMap;
		RenderingResourcePtr<RI_ShaderResourceView> m_pointLightShadowMapSRV;
		RenderingResourcePtr<RI_DepthStencilView>  m_pointLightShadowMapDSV;
		std::vector<RenderingResourcePtr<RI_RenderTargetView>> m_pointLightShadowMapRTV;
		uint32 m_pointLightShadowMapLastSlot;

		RenderingResourcePtr<RI_Texture2D> m_spotLightShadowMap;
		RenderingResourcePtr<RI_ShaderResourceView> m_spotLightShadowMapSRV;
		std::vector<RenderingResourcePtr<RI_DepthStencilView>> m_spotLightShadowMapDSV;
		uint32 m_spotLightShadowMapLastSlot;

		std::shared_ptr<Shader> m_standardShader;
		std::shared_ptr<Shader> m_standardTessellationShader;
		std::shared_ptr<Shader> m_shadowMapShader;
		std::shared_ptr<Shader> m_omnidirectionalShadowMapShader;
		std::shared_ptr<Shader> m_defaultSkyboxShader;
		std::shared_ptr<Mesh> m_defaultSphereMesh;
		std::shared_ptr<TextureCube> m_defaultSkyTexture;
		std::shared_ptr<Material> m_defaultSkyMaterial;
	};
}
