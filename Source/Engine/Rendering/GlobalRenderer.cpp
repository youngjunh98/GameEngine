#include "GlobalRenderer.h"
#include "Platform/Platform.h"
#include "Core/Modular/ModuleManager.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Transform.h"
#include "Engine/Component/Camera.h"
#include "Engine/Component/Rendering/Renderer.h"
#include "Engine/Component/Rendering/Light.h"
#include "Engine/Asset/AssetManager.h"

namespace GameEngine
{
	void ChangeResolution (uint32 width, uint32 height)
	{
		GlobalRenderer::ChangeScreenSize (width, height, false);
	}

	GlobalRenderer::GlobalRenderer () :
		m_maxLightCount (3)
	{
	}

	GlobalRenderer::~GlobalRenderer ()
	{
	}

	GlobalRenderer& GlobalRenderer::GetInstance ()
	{
		static GlobalRenderer instance;
		return instance;
	}

	bool GlobalRenderer::Init (const GlobalRendererSettings settings)
	{
		GlobalRenderer& instance = GetInstance ();
		instance.m_settings = settings;

		// Create and initialize rendering interface module
		PathString riModulePath = Modular::ModuleManager::GetModulePath ("D3D11RI");
		auto riModule = static_cast<RenderingInterface*> (Modular::ModuleManager::CreateModuleInstance (riModulePath));

		if (riModule == nullptr)
		{
			return false;
		}

		if (riModule->Initialize (settings.m_renderWidth, settings.m_renderHeight, settings.m_bFullScreenEnabled, settings.m_bVSyncEnabled, settings.m_refreshRate, settings.m_msaaCount) == false)
		{
			return false;
		}

		instance.m_riModulePath = riModulePath;
		instance.m_ri = riModule;

		// Set swap chain as render target
		if (UpdateSwapChainResource () == false)
		{
			return false;
		}

		if (UpdateScreenDepthStencilResource () == false)
		{
			return false;
		}

		SetRenderTargetAndDepthStencilAsDefault ();
		BindRenderTargetAndDepthStencil ();

		Vector2 swapChainSize;
		swapChainSize.m_x = static_cast<float> (GetSwapChainWidth ());
		swapChainSize.m_y = static_cast<float> (GetSwapChainHeight ());

		SetViewport (swapChainSize);

		instance.m_rsCullBack = riModule->CreateRasterizerState (EFillMode::Solid, EFaceCulling::Back, EWindingOrder::CW);

		if (instance.m_rsCullBack == nullptr)
		{
			return false;
		}

		instance.m_rsCullNone = riModule->CreateRasterizerState (EFillMode::Solid, EFaceCulling::None, EWindingOrder::CW);

		if (instance.m_rsCullNone == nullptr)
		{
			return false;
		}

		instance.m_depthLessStencilAlways = riModule->CreateDepthStencilState (EComparisonFunction::Less, EComparisonFunction::Always);

		if (instance.m_depthLessStencilAlways == nullptr)
		{
			return false;
		}

		instance.m_dssLessEqualStencilAlways = riModule->CreateDepthStencilState (EComparisonFunction::LessEqual, EComparisonFunction::Always);

		if (instance.m_dssLessEqualStencilAlways == nullptr)
		{
			return false;
		}

		instance.m_constantBufferPerObject = riModule->CreateShaderConstantBuffer (sizeof (ObjectConstantBuffer), nullptr);

		if (instance.m_constantBufferPerObject == nullptr)
		{
			return false;
		}

		instance.m_constantBufferPerCamera = riModule->CreateShaderConstantBuffer (sizeof (CameraConstantBuffer), nullptr);

		if (instance.m_constantBufferPerCamera == nullptr)
		{
			return false;
		}

		instance.m_lightingConstantBuffer = riModule->CreateShaderConstantBuffer (sizeof (LightingConstantBuffer), nullptr);

		if (instance.m_lightingConstantBuffer == nullptr)
		{
			return false;
		}

		Shader* skyboxShader = g_assetManager.LoadShader (L"Assets/Shader/Skybox.hlsl");
		TextureCube* textureCube = g_assetManager.FindTextureCubeAsset (L"Assets/skyRight.hdr", L"Assets/skyLeft.hdr",
			L"Assets/skyTop.hdr", L"Assets/skyBottom.hdr", L"Assets/skyFront.hdr", L"Assets/skyBack.hdr");
		instance.m_skyboxMaterial = new Material ();
		instance.m_skyboxMaterial->SetShader (skyboxShader);
		instance.m_skyboxMaterial->SetTexture ("Skybox", *textureCube);

		instance.m_shadowMapShader = g_assetManager.LoadShader (L"Assets/Shader/ShadowMapShader.hlsl");
		instance.m_omnidirectionalShadowMapShader = g_assetManager.LoadShader (L"Assets/Shader/OmnidirectionalShadowMapShader.hlsl");

		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		instance.m_shadowMapSampler = riModule->CreateSampler (EAddressMode::Border, EFilterMode::Linear, 1, borderColor);

		if (instance.m_shadowMapSampler == nullptr)
		{
			return false;
		}

		instance.m_directionalLightShadowMap = riModule->CreateTexture2D (2048, 2048, 1, 3, ERenderingResourceFormat::R24G8_Typeless, nullptr, nullptr, false, true, false, true);
		if (instance.m_directionalLightShadowMap == nullptr)
		{
			return false;
		}

		instance.m_directionalLightShadowMapSRV = riModule->CreateShaderResourceView (instance.m_directionalLightShadowMap.get (), ERenderingResourceFormat::R24_UNorm_X8_Typeless, 0, 3, 0);
		if (instance.m_directionalLightShadowMapSRV == nullptr)
		{
			return false;
		}

		for (int32 i = 0; i < 3; i++)
		{
			auto directionalShadowMapDSV = riModule->CreateDepthStencilView (instance.m_directionalLightShadowMap.get (), 0, 1, i);
			if (directionalShadowMapDSV == nullptr)
			{
				return false;
			}

			instance.m_directionalLightShadowMapDSV.push_back (directionalShadowMapDSV);
		}

		instance.m_pointLightShadowMap = riModule->CreateTexture2D (1024, 1024, 1, 6 * 3, ERenderingResourceFormat::R32_Float, nullptr, nullptr, true, true, true, false);
		if (instance.m_pointLightShadowMap == nullptr)
		{
			return false;
		}

		instance.m_pointLightShadowMapSRV = riModule->CreateShaderResourceView (instance.m_pointLightShadowMap.get (), ERenderingResourceFormat::R32_Float, 0, 3, 0);
		if (instance.m_pointLightShadowMapSRV == nullptr)
		{
			return false;
		}

		auto pointLightShadowMapDepthStencil = riModule->CreateTexture2D (1024, 1024, 1, 6 * 3, ERenderingResourceFormat::D24_UNorm_S8_UInt, nullptr, nullptr, false, false, false, true);

		instance.m_pointLightShadowMapDSV = riModule->CreateDepthStencilView (pointLightShadowMapDepthStencil.get (), 0, 1, 1);
		if (instance.m_pointLightShadowMapDSV == nullptr)
		{
			return false;
		}

		for (int32 i = 0; i < 6 * 3; i++)
		{
			auto pointShadowMapRTV = riModule->CreateRenderTargetView (instance.m_pointLightShadowMap.get (), 0, 1, i);
			if (pointShadowMapRTV == nullptr)
			{
				return false;
			}

			instance.m_pointLightShadowMapRTV.push_back (pointShadowMapRTV);
		}

		instance.m_spotLightShadowMap = riModule->CreateTexture2D (1024, 1024, 1, 3, ERenderingResourceFormat::R24G8_Typeless, nullptr, nullptr, false, true, false, true);
		if (instance.m_spotLightShadowMap == nullptr)
		{
			return false;
		}

		instance.m_spotLightShadowMapSRV = riModule->CreateShaderResourceView (instance.m_spotLightShadowMap.get (), ERenderingResourceFormat::R24_UNorm_X8_Typeless, 0, 3, 0);
		if (instance.m_spotLightShadowMapSRV == nullptr)
		{
			return false;
		}

		for (int32 i = 0; i < 3; i++)
		{
			auto spotLightShadowMapDSV = riModule->CreateDepthStencilView (instance.m_spotLightShadowMap.get (), 0, 1, i);
			if (spotLightShadowMapDSV == nullptr)
			{
				return false;
			}

			instance.m_spotLightShadowMapDSV.push_back (spotLightShadowMapDSV);
		}

		BindRenderPipeline (GetDefaultForwardRenderPipeline ());

		riModule->SetPrimitiveTopology (EPrimitiveTopology::TriangleList);

		Platform::GetGenericApplication ().AddResizeListener (ChangeResolution);

		return true;
	}

	void GlobalRenderer::Shutdown ()
	{
		Platform::GetGenericApplication ().RemoveResizeListener (ChangeResolution);
		GlobalRenderer& instance = GetInstance ();

		if (instance.m_skyboxMaterial != nullptr)
		{
			instance.m_skyboxMaterial->Destroy ();
			delete instance.m_skyboxMaterial;
			instance.m_skyboxMaterial = nullptr;
		}

		instance.m_shadowMapShader = nullptr;
		instance.m_omnidirectionalShadowMapShader = nullptr;

		instance.m_defaultForwardPipeline.Release ();
		instance.m_defaultLinePipeline.Release ();

		instance.m_swapChainBuffer = nullptr;
		instance.m_swapChainRenderTarget = nullptr;
		instance.m_screenDepthStencilBuffer = nullptr;
		instance.m_screenDepthStencil = nullptr;

		instance.m_usingRenderTarget = nullptr;
		instance.m_usingDepthStencil = nullptr;

		instance.m_rsCullBack = nullptr;
		instance.m_rsCullNone = nullptr;

		instance.m_depthLessStencilAlways = nullptr;
		instance.m_dssLessEqualStencilAlways = nullptr;

		instance.m_constantBufferPerObject = nullptr;
		instance.m_constantBufferPerCamera = nullptr;
		instance.m_lightingConstantBuffer = nullptr;

		instance.m_shadowMapSampler = nullptr;

		instance.m_directionalLightShadowMap = nullptr;
		instance.m_directionalLightShadowMapSRV = nullptr;
		instance.m_directionalLightShadowMapDSV.clear ();

		instance.m_pointLightShadowMap = nullptr;
		instance.m_pointLightShadowMapSRV = nullptr;
		instance.m_pointLightShadowMapDSV = nullptr;
		instance.m_pointLightShadowMapRTV.clear ();

		instance.m_spotLightShadowMap = nullptr;
		instance.m_spotLightShadowMapSRV = nullptr;
		instance.m_spotLightShadowMapDSV.clear ();

		instance.m_ri->Shutdown ();
		instance.m_ri = nullptr;

		Modular::ModuleManager::UnloadModule (instance.m_riModulePath);
	}

	void GlobalRenderer::RenderScene (Scene* scene)
	{
		if (scene == nullptr)
		{
			return;
		}

		GlobalRenderer& instance = GetInstance ();
		RenderPipeline& pipeline = *instance.m_renderPipeline;

		const auto& gameObjects = scene->GetGameObjects ();
		std::vector<Camera*> cameras;

		for (auto& gameObject : gameObjects)
		{
			if (gameObject->IsActive () == false)
			{
				continue;
			}

			auto cameraComponents = gameObject->GetComponents<Camera> ();
			cameras.insert (cameras.end (), cameraComponents.begin (), cameraComponents.end ());
		}

		for (auto* camera : cameras)
		{
			Vector3 cameraPosition = camera->GetGameObject ().GetComponent<Transform> ()->GetPosition ();
			Matrix4x4 view = camera->GetViewMatrix ();
			Matrix4x4 projection = camera->GetProjectionMatrix ();
			float cameraNear = camera->GetNear ();
			float cameraFar = camera->GetFar ();

			RenderScene (pipeline, view, projection, cameraPosition, cameraNear, cameraFar, scene);
		}
	}

	void GlobalRenderer::RenderScene (RenderPipeline& pipeline, Matrix4x4 viewMatrix, Matrix4x4 projectionMatrix, Vector3 cameraPosition, float cameraNear, float cameraFar, Scene* scene)
	{
		const auto& gameObjects = scene->GetGameObjects ();
		std::vector<Renderer*> renderers;
		std::vector<Light*> lights;

		for (auto& gameObject : gameObjects)
		{
			if (gameObject->IsActive () == false)
			{
				continue;
			}

			auto rendererComponents = gameObject->GetComponents<Renderer> ();
			renderers.insert (renderers.end (), rendererComponents.begin (), rendererComponents.end ());

			auto lightComponents = gameObject->GetComponents<Light> ();
			lights.insert (lights.end (), lightComponents.begin (), lightComponents.end ());
		}

		RenderPipelineData pipelineData;
		pipelineData.m_camera.m_viewMatrix = viewMatrix;
		pipelineData.m_camera.m_projectionMatrix = projectionMatrix;
		pipelineData.m_camera.m_position = cameraPosition;
		pipelineData.m_camera.m_near = cameraNear;
		pipelineData.m_camera.m_far = cameraFar;
		pipelineData.m_renderers = renderers;
		pipelineData.m_lights = lights;

		pipeline.Execute (pipelineData);
	}

	void GlobalRenderer::PresentSwapChain ()
	{
		GlobalRenderer& instance = GetInstance ();
		instance.m_ri->Present (instance.m_settings.m_bVSyncEnabled);
	}

	bool GlobalRenderer::ResizeSwapChain (uint32 width, uint32 height, bool bFullscreen)
	{
		GlobalRenderer& instance = GetInstance ();
		uint32 oldWidth = GetSwapChainWidth ();
		uint32 oldHeight = GetSwapChainHeight ();
		bool bUpdateSwapChain = (width >= 0 && height >= 0) && (width != oldWidth || height != oldHeight);

		if (bUpdateSwapChain)
		{
			bool bUsingSwapChainRenderTarget = instance.m_usingRenderTarget == instance.m_swapChainRenderTarget;

			if (bUsingSwapChainRenderTarget)
			{
				instance.m_usingRenderTarget = nullptr;
			}

			instance.m_swapChainRenderTarget = nullptr;
			instance.m_swapChainBuffer = nullptr;

			if (instance.m_ri->ResizeSwapChainBuffer (width, height, bFullscreen))
			{
				bool bUpdateResource = UpdateSwapChainResource ();

				if (bUsingSwapChainRenderTarget && bUpdateResource)
				{
					instance.m_usingRenderTarget = instance.m_swapChainRenderTarget;
				}
			}
		}

		return instance.m_swapChainBuffer != nullptr && instance.m_swapChainRenderTarget != nullptr;
	}

	bool GlobalRenderer::UpdateSwapChainResource ()
	{
		GlobalRenderer& instance = GetInstance ();
		instance.m_swapChainBuffer = instance.m_ri->GetSwapChainBuffer ();

		if (instance.m_swapChainBuffer != nullptr)
		{
			RI_Texture2D* swapChain = instance.m_swapChainBuffer.get ();
			uint32 arraySize = instance.m_swapChainBuffer->m_arraySize;

			instance.m_swapChainRenderTarget = instance.m_ri->CreateRenderTargetView (swapChain, 0, arraySize, 0);
		}

		return instance.m_swapChainBuffer != nullptr && instance.m_swapChainRenderTarget != nullptr;
	}

	RI_RenderTargetView* GlobalRenderer::GetSwapChainRenderTarget ()
	{
		return GetInstance ().m_swapChainRenderTarget.get ();
	}

	uint32 GlobalRenderer::GetSwapChainWidth ()
	{
		GlobalRenderer& instance = GetInstance ();
		return instance.m_swapChainBuffer != nullptr ? instance.m_swapChainBuffer->m_width : 0;
	}

	uint32 GlobalRenderer::GetSwapChainHeight ()
	{
		GlobalRenderer& instance = GetInstance ();
		return instance.m_swapChainBuffer != nullptr ? instance.m_swapChainBuffer->m_height : 0;
	}

	void GlobalRenderer::ChangeScreenSize (uint32 width, uint32 height, bool bFullscreen)
	{
		uint32 oldWidth = GetSwapChainWidth ();
		uint32 oldHeight = GetSwapChainHeight ();
		bool bUpdateScreen = (width > 0 && height > 0) && (width != oldWidth || height != oldHeight);

		if (bUpdateScreen)
		{
			if (ResizeSwapChain (width, height, bFullscreen))
			{
				if (UpdateScreenDepthStencilResource ())
				{
					GlobalRenderer& instance = GetInstance ();

					if (instance.m_swapChainRenderTarget == instance.m_usingRenderTarget)
					{
						instance.m_settings.m_renderWidth = width;
						instance.m_settings.m_renderHeight = height;
					}

					instance.m_settings.m_bFullScreenEnabled = bFullscreen;
				}
			}
		}
	}

	bool GlobalRenderer::UpdateScreenDepthStencilResource ()
	{
		GlobalRenderer& instance = GetInstance ();
		uint32 width = instance.m_swapChainBuffer->m_width;
		uint32 height = instance.m_swapChainBuffer->m_height;
		uint32 mipmapCount = instance.m_swapChainBuffer->m_mipmapCount;
		uint32 arraySize = instance.m_swapChainBuffer->m_arraySize;

		bool bUsingScreenDepthStencil = instance.m_usingDepthStencil == instance.m_screenDepthStencil;

		if (bUsingScreenDepthStencil)
		{
			instance.m_usingDepthStencil = nullptr;
		}

		instance.m_screenDepthStencilBuffer = instance.m_ri->CreateTexture2D (width, height, mipmapCount, arraySize, ERenderingResourceFormat::D24_UNorm_S8_UInt, nullptr, nullptr, false, false, false, true);
		instance.m_screenDepthStencil = instance.m_ri->CreateDepthStencilView (instance.m_screenDepthStencilBuffer.get (), 0, arraySize, 0);

		if (bUsingScreenDepthStencil && instance.m_screenDepthStencil != nullptr)
		{
			instance.m_usingDepthStencil = instance.m_screenDepthStencil;
		}

		return instance.m_screenDepthStencilBuffer != nullptr && instance.m_screenDepthStencil != nullptr;
	}

	RI_DepthStencilView* GlobalRenderer::GetScreenDepthStencil ()
	{
		return GetInstance ().m_screenDepthStencil.get ();
	}

	void GlobalRenderer::BindRenderTargetAndDepthStencil ()
	{
		GlobalRenderer& instance = GetInstance ();
		RI_RenderTargetView* renderTarget = instance.m_usingRenderTarget.get ();
		RI_DepthStencilView* depthStencil = instance.m_usingDepthStencil.get ();

		instance.m_ri->BindRenderTargetViewAndDepthStencilView (renderTarget, depthStencil);
	}

	void GlobalRenderer::ClearRenderTargetAndDepthStencil (Vector4 color, float depth, uint8 stencil)
	{
		GlobalRenderer& instance = GetInstance ();
		RI_RenderTargetView* renderTarget = instance.m_usingRenderTarget.get ();
		RI_DepthStencilView* depthStencil = instance.m_usingDepthStencil.get ();

		instance.m_ri->ClearRenderTarget (renderTarget, color.m_x, color.m_y, color.m_z, color.m_w);
		instance.m_ri->ClearDepthStencil (depthStencil, depth, stencil);
	}

	RenderingResourcePtr<RI_RenderTargetView> GlobalRenderer::GetRenderTarget ()
	{
		return GetInstance ().m_usingRenderTarget;
	}

	RenderingResourcePtr<RI_DepthStencilView> GlobalRenderer::GetDepthStencil ()
	{
		return GetInstance ().m_usingDepthStencil;
	}

	void GlobalRenderer::SetRenderTargetAndDepthStencilAsDefault ()
	{
		GlobalRenderer& instance = GetInstance ();
		SetRenderTarget (instance.m_swapChainRenderTarget);
		SetDepthStencil (instance.m_screenDepthStencil);
	}

	void GlobalRenderer::SetRenderTarget (RenderingResourcePtr<RI_RenderTargetView> renderTarget)
	{
		GetInstance ().m_usingRenderTarget = renderTarget;
	}

	void GlobalRenderer::SetDepthStencil (RenderingResourcePtr<RI_DepthStencilView> depthStencil)
	{
		GetInstance ().m_usingDepthStencil = depthStencil;
	}

	Vector2 GlobalRenderer::GetRenderSize ()
	{
		GlobalRenderer& instance = GetInstance ();
		float width = static_cast<float> (instance.m_settings.m_renderWidth);
		float height = static_cast<float> (instance.m_settings.m_renderHeight);

		return Vector2 (width, height);
	}

	void GlobalRenderer::SetRenderSize (Vector2 size)
	{
		GlobalRenderer& instance = GetInstance ();
		instance.m_settings.m_renderWidth = static_cast<uint32> (size.m_x);
		instance.m_settings.m_renderHeight = static_cast<uint32> (size.m_y);
	}

	Vector2 GlobalRenderer::GetViewportSize ()
	{
		return GetInstance ().m_viewportSize;
	}

	Vector2 GlobalRenderer::GetViewportTopLeft ()
	{
		return GetInstance ().m_viewportTopLeft;
	}

	void GlobalRenderer::SetViewport (Vector2 size, Vector2 topLeft)
	{
		GlobalRenderer& instance = GetInstance ();

		if (instance.m_ri->SetViewport (size.m_x, size.m_y, topLeft.m_x, topLeft.m_y))
		{
			instance.m_viewportSize = size;
			instance.m_viewportTopLeft = topLeft;
		}
	}

	void GlobalRenderer::ActivateShadowMapShader (ELightType lightType)
	{
		GlobalRenderer& instance = GetInstance ();
		Shader* shader = nullptr;

		if (lightType == ELightType::Directional || lightType == ELightType::Spot)
		{
			shader = instance.m_shadowMapShader;
		}
		else if (lightType == ELightType::Point)
		{
			shader = instance.m_omnidirectionalShadowMapShader;
		}

		if (shader != nullptr)
		{
			instance.m_ri->SetInputLayout (shader->GetInputLayout ());
			instance.m_ri->ActivateVertexShader (shader->GetVertexShader ());
			instance.m_ri->ActivatePixelShader (shader->GetPixelShader ());
			instance.m_ri->ActivateHullShader (shader->GetHullShader ());
			instance.m_ri->ActivateDomainShader (shader->GetDomainShader ());
			instance.m_ri->ActivateGeometryShader (shader->GetGeometryShader ());

			BindGlobalShaderConstantBuffer (shader, "CBObject");
			BindGlobalShaderConstantBuffer (shader, "CBCamera");
			BindGlobalShaderConstantBuffer (shader, "CBLighting");
		}
	}

	void GlobalRenderer::BindShadowRenderTarget (ELightType lightType, uint32 renderTargetIndex)
	{
		GlobalRenderer& instance = GetInstance ();
		
		if (lightType == ELightType::Directional)
		{
			if (renderTargetIndex >= instance.m_directionalLightShadowMapDSV.size ())
			{
				return;
			}

			instance.m_ri->BindPixelShaderResource (nullptr, instance.m_directionalLightShadowMapLastSlot);

			RI_DepthStencilView* shadowDepthStencil = instance.m_directionalLightShadowMapDSV.at (renderTargetIndex).get ();
			instance.m_ri->BindRenderTargetViewAndDepthStencilView (nullptr, shadowDepthStencil);
			instance.m_ri->ClearDepthStencil (shadowDepthStencil, 1.0f, 0);

			instance.m_ri->SetViewport (2048, 2048, 0, 0);
		}
		else if (lightType == ELightType::Point)
		{
			if (renderTargetIndex >= instance.m_pointLightShadowMapRTV.size ())
			{
				return;
			}

			instance.m_ri->BindPixelShaderResource (nullptr, instance.m_pointLightShadowMapLastSlot);

			RI_RenderTargetView* renderTarget = instance.m_pointLightShadowMapRTV.at (renderTargetIndex).get ();
			instance.m_ri->BindRenderTargetViewAndDepthStencilView (renderTarget, instance.m_pointLightShadowMapDSV.get ());
			instance.m_ri->ClearRenderTarget (renderTarget, 0.0f, 0.0f, 0.0f, 0.0f);
			instance.m_ri->ClearDepthStencil (instance.m_pointLightShadowMapDSV.get (), 1.0f, 0);

			instance.m_ri->SetViewport (1024, 1024, 0, 0);
		}
		else if (lightType == ELightType::Spot)
		{
			if (renderTargetIndex >= instance.m_spotLightShadowMapDSV.size ())
			{
				return;
			}

			instance.m_ri->BindPixelShaderResource (nullptr, instance.m_spotLightShadowMapLastSlot);

			RI_DepthStencilView* shadowDepthStencil = instance.m_spotLightShadowMapDSV.at (renderTargetIndex).get ();
			instance.m_ri->BindRenderTargetViewAndDepthStencilView (nullptr, shadowDepthStencil);
			instance.m_ri->ClearDepthStencil (shadowDepthStencil, 1.0f, 0);

			instance.m_ri->SetViewport (1024, 1024, 0, 0);
		}
	}

	void GlobalRenderer::BindRenderPipeline (RenderPipeline* renedrPipeline)
	{
		GetInstance ().m_renderPipeline = renedrPipeline;
	}

	DefaultForwardRenderPipeline* GlobalRenderer::GetDefaultForwardRenderPipeline ()
	{
		return &GetInstance ().m_defaultForwardPipeline;
	}

	DefaultLineRenderPipeline* GlobalRenderer::GetDefaultLineRenderPipeline ()
	{
		return &GetInstance ().m_defaultLinePipeline;
	}

	void GlobalRenderer::BindMaterial (Material* material)
	{
		if (material == nullptr)
		{
			return;
		}

		Shader* shader = material->GetShader ();

		if (shader == nullptr)
		{
			return;
		}

		GlobalRenderer& instance = GetInstance ();
		instance.m_ri->SetInputLayout (shader->GetInputLayout ());
		instance.m_ri->ActivateVertexShader (shader->GetVertexShader ());
		instance.m_ri->ActivatePixelShader (shader->GetPixelShader ());
		instance.m_ri->ActivateHullShader (shader->GetHullShader ());
		instance.m_ri->ActivateDomainShader (shader->GetDomainShader ());
		instance.m_ri->ActivateGeometryShader (shader->GetGeometryShader ());

		BindGlobalShaderConstantBuffer (shader, "CBObject");
		BindGlobalShaderConstantBuffer (shader, "CBCamera");
		BindGlobalShaderConstantBuffer (shader, "CBLighting");

		// Vertex bindings
		if (shader->IsShaderResourceExist (EShaderStage::Vertex, "$Globals"))
		{
			instance.m_ri->UpdateShaderConstantBuffer (shader->GetVertexShaderParameterBuffer (), material->GetVertexShaderParameterData ());
			instance.m_ri->BindVertexShaderConstantBuffer (shader->GetVertexShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Vertex, "$Globals").m_bindIndex);
		}

		// Pixel bindings
		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "$Globals"))
		{
			instance.m_ri->UpdateShaderConstantBuffer (shader->GetPixelShaderParameterBuffer (), material->GetPixelShaderParameterData ());
			instance.m_ri->BindPixelShaderConstantBuffer (shader->GetPixelShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Pixel, "$Globals").m_bindIndex);
		}

		// Hull bindings
		if (shader->IsShaderResourceExist (EShaderStage::Hull, "$Globals"))
		{
			instance.m_ri->UpdateShaderConstantBuffer (shader->GetHullShaderParameterBuffer (), material->GetHullShaderParameterData ());
			instance.m_ri->BindHullShaderConstantBuffer (shader->GetHullShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Hull, "$Globals").m_bindIndex);
		}

		// Domain bindings
		if (shader->IsShaderResourceExist (EShaderStage::Domain, "$Globals"))
		{
			instance.m_ri->UpdateShaderConstantBuffer (shader->GetDomainShaderParameterBuffer (), material->GetDomainShaderParameterData ());
			instance.m_ri->BindDomainShaderConstantBuffer (shader->GetDomainShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Domain, "$Globals").m_bindIndex);
		}

		// Geometry bindings
		if (shader->IsShaderResourceExist (EShaderStage::Geometry, "$Globals"))
		{
			instance.m_ri->UpdateShaderConstantBuffer (shader->GetGeometryShaderParameterBuffer (), material->GetGeometryShaderParameterData ());
			instance.m_ri->BindGeometryShaderConstantBuffer (shader->GetDomainShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Geometry, "$Globals").m_bindIndex);
		}

		for (const auto& textureBinding : material->GetTextureMap ())
		{
			const std::string& name = textureBinding.first;
			Texture* texture = textureBinding.second;

			if (texture != nullptr)
			{
				if (shader->IsShaderResourceExist (EShaderStage::Vertex, name))
				{
					instance.m_ri->BindVertexShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Vertex, name).m_bindIndex);
					instance.m_ri->BindVertexShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Vertex, name + "Sampler").m_bindIndex);
				}

				if (shader->IsShaderResourceExist (EShaderStage::Pixel, name))
				{
					instance.m_ri->BindPixelShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Pixel, name).m_bindIndex);
					instance.m_ri->BindPixelShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Pixel, name + "Sampler").m_bindIndex);
				}

				if (shader->IsShaderResourceExist (EShaderStage::Hull, name))
				{
					instance.m_ri->BindHullShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Hull, name).m_bindIndex);
					instance.m_ri->BindHullShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Hull, name + "Sampler").m_bindIndex);
				}

				if (shader->IsShaderResourceExist (EShaderStage::Domain, name))
				{
					instance.m_ri->BindDomainShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Domain, name).m_bindIndex);
					instance.m_ri->BindDomainShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Domain, name + "Sampler").m_bindIndex);
				}

				if (shader->IsShaderResourceExist (EShaderStage::Geometry, name))
				{
					instance.m_ri->BindGeometryShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Geometry, name).m_bindIndex);
					instance.m_ri->BindGeometryShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Geometry, name + "Sampler").m_bindIndex);
				}
			}
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_DirectionalLightShadowMap"))
		{
			instance.m_directionalLightShadowMapLastSlot = shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_DirectionalLightShadowMap").m_bindIndex;
			instance.m_ri->BindPixelShaderResource (instance.m_directionalLightShadowMapSRV.get (), instance.m_directionalLightShadowMapLastSlot);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_PointLightShadowMap"))
		{
			instance.m_pointLightShadowMapLastSlot = shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_PointLightShadowMap").m_bindIndex;
			instance.m_ri->BindPixelShaderResource (instance.m_pointLightShadowMapSRV.get (), instance.m_pointLightShadowMapLastSlot);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_SpotLightShadowMap"))
		{
			instance.m_spotLightShadowMapLastSlot = shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_SpotLightShadowMap").m_bindIndex;
			instance.m_ri->BindPixelShaderResource (instance.m_spotLightShadowMapSRV.get (), instance.m_spotLightShadowMapLastSlot);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_ShadowMapSampler"))
		{
			instance.m_ri->BindPixelShaderSampler (instance.m_shadowMapSampler.get (), shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_ShadowMapSampler").m_bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_EnvironmentTexture"))
		{
			instance.m_ri->BindPixelShaderResource (instance.m_skyboxMaterial->GetTextureMap ().at ("Skybox")->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_EnvironmentTexture").m_bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_EnvironmentTextureSampler"))
		{
			instance.m_ri->BindPixelShaderSampler (instance.m_skyboxMaterial->GetTextureMap ().at ("Skybox")->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_EnvironmentTextureSampler").m_bindIndex);
		}
	}

	void GlobalRenderer::BindGlobalShaderConstantBuffer (Shader* shader, const std::string& name)
	{
		if (shader == nullptr)
		{
			return;
		}

		GlobalRenderer& instance = GetInstance ();
		RI_ShaderConstantBuffer* buffer = nullptr;
		uint32 bindIndex = 0;

		if (name == "CBObject")
		{
			buffer = instance.m_constantBufferPerObject.get ();
		}
		else if (name == "CBCamera")
		{
			buffer = instance.m_constantBufferPerCamera.get ();
		}
		else if (name == "CBLighting")
		{
			buffer = instance.m_lightingConstantBuffer.get ();
		}

		if (buffer == nullptr)
		{
			return;
		}

		if (shader->IsShaderResourceExist (EShaderStage::Vertex, name))
		{
			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Vertex, name).m_bindIndex;
			instance.m_ri->BindVertexShaderConstantBuffer (buffer, bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, name))
		{
			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Pixel, name).m_bindIndex;
			instance.m_ri->BindPixelShaderConstantBuffer (buffer, bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Hull, name))
		{
			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Hull, name).m_bindIndex;
			instance.m_ri->BindHullShaderConstantBuffer (buffer, bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Domain, name))
		{
			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Domain, name).m_bindIndex;
			instance.m_ri->BindDomainShaderConstantBuffer (buffer, bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Geometry, name))
		{
			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Geometry, name).m_bindIndex;
			instance.m_ri->BindGeometryShaderConstantBuffer (buffer, bindIndex);
		}
	}

	void GlobalRenderer::SetGlobalShaderConstantBuffer (const std::string& name, const void* bufferData)
	{
		GlobalRenderer& instance = GetInstance ();

		if (name == "CBObject")
		{
			instance.m_ri->UpdateShaderConstantBuffer (instance.m_constantBufferPerObject.get (), bufferData);
		}
		else if (name == "CBCamera")
		{
			instance.m_ri->UpdateShaderConstantBuffer (instance.m_constantBufferPerCamera.get (), bufferData);
		}
		else if (name == "CBLighting")
		{
			instance.m_ri->UpdateShaderConstantBuffer (instance.m_lightingConstantBuffer.get (), bufferData);
		};
	}

	void GlobalRenderer::DrawVertices (RI_VertexBuffer* vertexBuffer, RI_IndexBuffer* indexBuffer)
	{
		GlobalRenderer& instance = GetInstance ();

		if (indexBuffer == nullptr)
		{
			instance.m_ri->Draw (vertexBuffer);
		}
		else
		{
			instance.m_ri->DrawIndexed (vertexBuffer, indexBuffer);
		}
	}

	void GlobalRenderer::DrawSkybox ()
	{
		if (Camera::Main == nullptr)
		{
			return;
		}

		GlobalRenderer& instance = GetInstance ();

		instance.m_ri->SetRasterizerState (instance.m_rsCullNone.get ());
		instance.m_ri->SetDepthStencilState (instance.m_dssLessEqualStencilAlways.get ());

		instance.m_ri->ActivateVertexShader (instance.m_skyboxMaterial->GetShader ()->GetVertexShader ());
		instance.m_ri->ActivatePixelShader (instance.m_skyboxMaterial->GetShader ()->GetPixelShader ());

		float cameraNear = Camera::Main->GetNear ();
		float cameraFar = Camera::Main->GetFar ();
		float cameraFov = Camera::Main->GetMode () == ECameraMode::Orthographic ? 90.0f : Camera::Main->GetFieldOfView ();
		float aspectRatio = Camera::Main->GetAspectRatio ();

		Matrix4x4 cameraViewTransposed = Camera::Main->GetViewMatrix ().Transposed ();
		Matrix4x4 cameraProjectionTransposed = Matrix4x4::Perspective (cameraFov, aspectRatio, cameraNear, cameraFar).Transposed ();

		Vector3 cameraPosition = Camera::Main->GetGameObject ().GetTransform ().GetPosition ();

		CameraConstantBuffer cameraConstantBuffer;
		cameraConstantBuffer.m_view = cameraViewTransposed;
		cameraConstantBuffer.m_projection = cameraProjectionTransposed;
		cameraConstantBuffer.m_viewProjection = cameraProjectionTransposed * cameraViewTransposed;
		cameraConstantBuffer.m_cameraWorldPosition = cameraPosition;
		cameraConstantBuffer.m_cameraFar = cameraFar;

		GlobalRenderer::SetGlobalShaderConstantBuffer ("CBCamera", &cameraConstantBuffer);

		auto* skyMesh = g_assetManager.FindAsset<Mesh> (L"Assets/sphere.obj");

		instance.m_ri->SetInputLayout (instance.m_skyboxMaterial->GetShader ()->GetInputLayout ());
		instance.m_ri->BindVertexShaderConstantBuffer (instance.m_constantBufferPerCamera.get (), instance.m_skyboxMaterial->GetShader ()->GetShaderResourceBinding (EShaderStage::Vertex, "CBCamera").m_bindIndex);
		instance.m_ri->BindPixelShaderResource (instance.m_skyboxMaterial->GetTextureMap ().find ("Skybox")->second->GetSRV (), instance.m_skyboxMaterial->GetShader ()->GetShaderResourceBinding (EShaderStage::Pixel, "Skybox").m_bindIndex);
		instance.m_ri->BindPixelShaderSampler (instance.m_skyboxMaterial->GetTextureMap ().find ("Skybox")->second->GetSampler (), instance.m_skyboxMaterial->GetShader ()->GetShaderResourceBinding (EShaderStage::Pixel, "SkyboxSampler").m_bindIndex);

		DrawVertices (skyMesh->GetVertexBufferResource (0), skyMesh->GetIndexBufferResource (0));

		instance.m_ri->SetRasterizerState (instance.m_rsCullBack.get ());
		instance.m_ri->SetDepthStencilState (instance.m_depthLessStencilAlways.get ());
	}

	LightData GlobalRenderer::GetLightData (uint32 index)
	{
		GlobalRenderer& instance = GetInstance ();
		LightData data;

		if (index < instance.m_maxLightCount)
		{
			data = instance.m_lightData.at (index);
		}

		return data;
	}

	void GlobalRenderer::AddLightData (LightData data)
	{
		GlobalRenderer& instance = GetInstance ();

		if (instance.m_lightData.size () >= instance.m_maxLightCount)
		{
			return;
		}

		instance.m_lightData.push_back (data);
	}

	void GlobalRenderer::ResetLightData ()
	{
		GetInstance ().m_lightData.clear ();
	}

	void GlobalRenderer::UpdateLightDataBuffer ()
	{
		GlobalRenderer& instance = GetInstance ();
		LightingConstantBuffer lightingConstantBuffer;
		lightingConstantBuffer.m_lightCount = Math::Min (static_cast<int32> (instance.m_lightData.size ()), instance.m_maxLightCount);
		lightingConstantBuffer.m_shadowDistance = 0.0f;

		for (int32 i = 0; i < lightingConstantBuffer.m_lightCount; i++)
		{
			lightingConstantBuffer.m_data[i] = instance.m_lightData.at (i);
		}

		SetGlobalShaderConstantBuffer ("CBLighting", &lightingConstantBuffer);
	}

	int32 GlobalRenderer::GetLightCount ()
	{
		return GetInstance ().m_lightData.size ();
	}

	int32 GlobalRenderer::GetMaxLightCount ()
	{
		return GetInstance ().m_maxLightCount;
	}

	RenderingInterface& GlobalRenderer::GetRenderingInterface ()
	{
		return *GetInstance ().m_ri;
	}

	GlobalRendererSettings GlobalRenderer::GetSettings ()
	{
		return GetInstance ().m_settings;
	}
}
