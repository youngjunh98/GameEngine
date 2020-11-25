#pragma once

#include <vector>
#include <memory>

#include "EngineType.h"
#include "Platform/PlatformRenderingInterface.h"
#include "Core/CoreMinimal.h"
#include "Core/File/FileSystem.h"
#include "Rendering/Pipeline/RenderPipeline.h"
#include "Rendering/Pipeline/Forward/DefaultForwardRenderPipeline.h"
#include "Rendering/Pipeline/Line/DefaultLineRenderPipeline.h"
#include "Rendering/GlobalRendererSettings.h"
#include "Rendering/TextureCube.h"

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

	class GlobalRenderer
	{
	public:
		GlobalRenderer ();
		virtual ~GlobalRenderer ();

		bool Init (const GlobalRendererSettings settings);
		void Shutdown ();

		void ActivateShadowMapShader (int32 lightType);
		void BindShadowRenderTarget (int32 lightType, uint32 renderTargetIndex);

		void ChangeScreenResolution (uint32 width, uint32 height, bool bFullscreenEnabled);
		bool ResetScreenRenderTarget ();
		RI_RenderTargetView* GetScreenRenderTarget () const;
		RI_DepthStencilView* GetScreenDepthStencil () const;

		RI_RenderTargetView* GetRenderTarget () const;
		RI_DepthStencilView* GetDepthStencil () const;
		void SetRenderTarget (RenderingResourcePtr<RI_RenderTargetView> renderTarget, RenderingResourcePtr<RI_DepthStencilView> depthStencil);

		GlobalRendererSettings GetSettings () const;

		void BindRenderPipeline (RenderPipeline* renedrPipeline);
		DefaultForwardRenderPipeline* GetDefaultForwardRenderPipeline ();
		DefaultLineRenderPipeline* GetDefaultLineRenderPipeline ();

		void BindRenderTarget (RI_RenderTargetView* renderTarget, RI_DepthStencilView* depthStencil);
		void ClearRenderTarget (RI_RenderTargetView* renderTarget, RI_DepthStencilView* depthStencil, Vector4 color, float depth, uint8 stencil);

		void BindMaterial (Material* material);

		void BindGlobalShaderConstantBuffer (Shader* shader, const std::string& name);
		void SetGlobalShaderConstantBuffer (const std::string& name, const void* bufferData);

		void DrawVertices (RI_VertexBuffer* vertexBuffer, RI_IndexBuffer* indexBuffer);

		void PresentScreen ();
		void RenderScene (Scene* scene);
		CullData Cull (Camera* camera, const std::vector<Renderer*>& renderers);
		void DrawSkybox ();

		LightData GetLightData (uint32 index);
		void AddLightData (LightData data);
		void ResetLightData ();
		void UpdateLightDataBuffer ();

		int32 GetLightCount () const;
		int32 GetMaxLightCount () const;

		Vector2 GetScreenRenderSize () const;
		bool IsVSyncEnabled () const;

		PlatformRenderingInterface& GetPlatformRenderingInterface ();

	private:
		GlobalRendererSettings m_settings;
		PlatformRenderingInterface* m_ri;
		PathString m_riModulePath;

		int32 m_maxLightCount;
		std::vector<LightData> m_lightData;
		LightingConstantBuffer m_lightBuffer;

		RenderPipeline* m_renderPipeline;
		DefaultForwardRenderPipeline m_defaultForwardPipeline;
		DefaultLineRenderPipeline m_defaultLinePipeline;

		RenderingResourcePtr<RI_Texture2D> m_swapChain;
		RenderingResourcePtr<RI_RenderTargetView> m_swapChainRenderTarget;
		RenderingResourcePtr<RI_DepthStencilView> m_swapChainDepthStencil;

		RenderingResourcePtr<RI_RenderTargetView> m_renderTarget;
		RenderingResourcePtr<RI_DepthStencilView> m_depthStencil;

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

		Shader* m_shadowMapShader;
		Shader* m_omnidirectionalShadowMapShader;

		Material* m_skyboxMaterial;
	};

	extern GlobalRenderer g_renderer;
}
