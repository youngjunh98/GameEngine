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

		void PresentSwapChain ();
		bool ResizeSwapChain (uint32 width, uint32 height, bool bFullscreen);
		bool UpdateSwapChainResource ();
		RI_RenderTargetView* GetSwapChainRenderTarget () const;
		uint32 GetSwapChainWidth () const;
		uint32 GetSwapChainHeight () const;

		void ChangeScreenSize (uint32 width, uint32 height, bool bFullscreen);
		bool UpdateScreenDepthStencilResource ();
		RI_DepthStencilView* GetScreenDepthStencil () const;

		void BindRenderTargetAndDepthStencil ();
		void ClearRenderTargetAndDepthStencil (Vector4 color, float depth, uint8 stencil);
		RI_RenderTargetView* GetRenderTarget () const;
		RI_DepthStencilView* GetDepthStencil () const;
		void SetRenderTargetAndDepthStencilAsDefault ();
		void SetRenderTarget (RenderingResourcePtr<RI_RenderTargetView> renderTarget);
		void SetDepthStencil (RenderingResourcePtr<RI_DepthStencilView> depthStencil);

		Vector2 GetRenderSize () const;
		void SetRenderSize (Vector2 size);

		Vector2 GetViewportSize () const;
		Vector2 GetViewportTopLeft () const;
		void SetViewport (Vector2 size, Vector2 topLeft = Vector2::Zero);

		void ActivateShadowMapShader (int32 lightType);
		void BindShadowRenderTarget (int32 lightType, uint32 renderTargetIndex);

		GlobalRendererSettings GetSettings () const;

		void BindRenderPipeline (RenderPipeline* renedrPipeline);
		DefaultForwardRenderPipeline* GetDefaultForwardRenderPipeline ();
		DefaultLineRenderPipeline* GetDefaultLineRenderPipeline ();

		void BindMaterial (Material* material);
		void BindGlobalShaderConstantBuffer (Shader* shader, const std::string& name);
		void SetGlobalShaderConstantBuffer (const std::string& name, const void* bufferData);

		void DrawVertices (RI_VertexBuffer* vertexBuffer, RI_IndexBuffer* indexBuffer);

		void RenderScene (Scene* scene);
		CullData Cull (Camera* camera, const std::vector<Renderer*>& renderers);
		void DrawSkybox ();

		LightData GetLightData (uint32 index);
		void AddLightData (LightData data);
		void ResetLightData ();
		void UpdateLightDataBuffer ();

		int32 GetLightCount () const;
		int32 GetMaxLightCount () const;

		PlatformRenderingInterface& GetPlatformRenderingInterface ();

	private:
		GlobalRendererSettings m_settings;

		PlatformRenderingInterface* m_ri;
		PathString m_riModulePath;

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

		Shader* m_shadowMapShader;
		Shader* m_omnidirectionalShadowMapShader;

		Material* m_skyboxMaterial;
	};

	extern GlobalRenderer g_renderer;
}
