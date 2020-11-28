#include "Platform/Platform.h"
#include "Core/Modular/ModuleManager.h"
#include "Rendering/GlobalRenderer.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "GameObject.h"
#include "Component/Camera.h"
#include "Component/Transform.h"
#include "Component/Rendering/Light.h"
#include "Component/Rendering/Renderer.h"
#include "Asset/AssetManager.h"

namespace GameEngine
{
	GlobalRenderer g_renderer;

	void ChangeResolution (uint32 width, uint32 height)
	{
		g_renderer.ChangeScreenSize (width, height, false);
	}

	GlobalRenderer::GlobalRenderer () :
		m_maxLightCount (3)
	{
	}

	GlobalRenderer::~GlobalRenderer ()
	{
	}

	bool GlobalRenderer::Init (const GlobalRendererSettings settings)
	{
		m_settings = settings;

		// Create and initialize rendering interface module
		m_riModulePath = Modular::ModuleManager::GetModulePath ("D3D11RI");
		m_ri = reinterpret_cast<PlatformRenderingInterface*> (Modular::ModuleManager::CreateModuleInstance (m_riModulePath));

		if (m_ri == nullptr)
		{
			return false;
		}

		if (m_ri->Initialize (settings.m_renderWidth, settings.m_renderHeight, settings.m_bFullScreenEnabled, settings.m_bVSyncEnabled, settings.m_refreshRate, settings.m_msaaCount) == false)
		{
			return false;
		}

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

		m_rsCullBack = m_ri->CreateRasterizerState (EFillMode::Solid, EFaceCulling::Back, EWindingOrder::CW);

		if (m_rsCullBack == nullptr)
		{
			return false;
		}

		m_rsCullNone = m_ri->CreateRasterizerState (EFillMode::Solid, EFaceCulling::None, EWindingOrder::CW);

		if (m_rsCullNone == nullptr)
		{
			return false;
		}

		m_depthLessStencilAlways = m_ri->CreateDepthStencilState (EComparisonFunction::Less, EComparisonFunction::Always);

		if (m_depthLessStencilAlways == nullptr)
		{
			return false;
		}

		m_dssLessEqualStencilAlways = m_ri->CreateDepthStencilState (EComparisonFunction::LessEqual, EComparisonFunction::Always);

		if (m_dssLessEqualStencilAlways == nullptr)
		{
			return false;
		}

		m_constantBufferPerObject = m_ri->CreateShaderConstantBuffer (sizeof (ObjectConstantBuffer), nullptr);

		if (m_constantBufferPerObject == nullptr)
		{
			return false;
		}

		m_constantBufferPerCamera = m_ri->CreateShaderConstantBuffer (sizeof (CameraConstantBuffer), nullptr);

		if (m_constantBufferPerCamera == nullptr)
		{
			return false;
		}

		m_lightingConstantBuffer = m_ri->CreateShaderConstantBuffer (sizeof (LightingConstantBuffer), nullptr);

		if (m_lightingConstantBuffer == nullptr)
		{
			return false;
		}

		Shader* skyboxShader = g_assetManager.LoadShader (L"Assets/Shader/Skybox.hlsl");
		TextureCube* textureCube = g_assetManager.FindTextureCubeAsset (L"Assets/skyRight.hdr", L"Assets/skyLeft.hdr",
			L"Assets/skyTop.hdr", L"Assets/skyBottom.hdr", L"Assets/skyFront.hdr", L"Assets/skyBack.hdr");
		m_skyboxMaterial = new Material ();
		m_skyboxMaterial->SetShader (skyboxShader);
		m_skyboxMaterial->SetTexture ("Skybox", *textureCube);

		m_shadowMapShader = g_assetManager.LoadShader (L"Assets/Shader/ShadowMapShader.hlsl");
		m_omnidirectionalShadowMapShader = g_assetManager.LoadShader (L"Assets/Shader/OmnidirectionalShadowMapShader.hlsl");

		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_shadowMapSampler = m_ri->CreateSampler (EAddressMode::Border, EFilterMode::Linear, 1, borderColor);

		if (m_shadowMapSampler == nullptr)
		{
			return false;
		}

		m_directionalLightShadowMap = m_ri->CreateTexture2D (2048, 2048, 1, 3, ERenderingResourceFormat::R24G8_Typeless, nullptr, nullptr, false, true, false, true);
		if (m_directionalLightShadowMap == nullptr)
		{
			return false;
		}

		m_directionalLightShadowMapSRV = m_ri->CreateShaderResourceView (m_directionalLightShadowMap.get (), ERenderingResourceFormat::R24_UNorm_X8_Typeless, 0, 3, 0);
		if (m_directionalLightShadowMapSRV == nullptr)
		{
			return false;
		}

		for (int32 i = 0; i < 3; i++)
		{
			auto directionalShadowMapDSV = m_ri->CreateDepthStencilView (m_directionalLightShadowMap.get (), 0, 1, i);
			if (directionalShadowMapDSV == nullptr)
			{
				return false;
			}

			m_directionalLightShadowMapDSV.push_back (directionalShadowMapDSV);
		}

		m_pointLightShadowMap = m_ri->CreateTexture2D (1024, 1024, 1, 6 * 3, ERenderingResourceFormat::R32_Float, nullptr, nullptr, true, true, true, false);
		if (m_pointLightShadowMap == nullptr)
		{
			return false;
		}

		m_pointLightShadowMapSRV = m_ri->CreateShaderResourceView (m_pointLightShadowMap.get (), ERenderingResourceFormat::R32_Float, 0, 3, 0);
		if (m_pointLightShadowMapSRV == nullptr)
		{
			return false;
		}

		auto pointLightShadowMapDepthStencil = m_ri->CreateTexture2D (1024, 1024, 1, 6 * 3, ERenderingResourceFormat::D24_UNorm_S8_UInt, nullptr, nullptr, false, false, false, true);

		m_pointLightShadowMapDSV = m_ri->CreateDepthStencilView (pointLightShadowMapDepthStencil.get (), 0, 1, 1);
		if (m_pointLightShadowMapDSV == nullptr)
		{
			return false;
		}

		for (int32 i = 0; i < 6 * 3; i++)
		{
			auto pointShadowMapRTV = m_ri->CreateRenderTargetView (m_pointLightShadowMap.get (), 0, 1, i);
			if (pointShadowMapRTV == nullptr)
			{
				return false;
			}

			m_pointLightShadowMapRTV.push_back (pointShadowMapRTV);
		}

		m_spotLightShadowMap = m_ri->CreateTexture2D (1024, 1024, 1, 3, ERenderingResourceFormat::R24G8_Typeless, nullptr, nullptr, false, true, false, true);
		if (m_spotLightShadowMap == nullptr)
		{
			return false;
		}

		m_spotLightShadowMapSRV = m_ri->CreateShaderResourceView (m_spotLightShadowMap.get (), ERenderingResourceFormat::R24_UNorm_X8_Typeless, 0, 3, 0);
		if (m_spotLightShadowMapSRV == nullptr)
		{
			return false;
		}

		for (int32 i = 0; i < 3; i++)
		{
			auto spotLightShadowMapDSV = m_ri->CreateDepthStencilView (m_spotLightShadowMap.get (), 0, 1, i);
			if (spotLightShadowMapDSV == nullptr)
			{
				return false;
			}

			m_spotLightShadowMapDSV.push_back (spotLightShadowMapDSV);
		}

		BindRenderPipeline (GetDefaultForwardRenderPipeline ());

		m_ri->SetPrimitiveTopology (EPrimitiveTopology::TriangleList);

		Platform::GetGenericApplication ().AddResizeListener (ChangeResolution);

		return true;
	}

	void GlobalRenderer::Shutdown ()
	{
		Platform::GetGenericApplication ().RemoveResizeListener (ChangeResolution);

		if (m_skyboxMaterial != nullptr)
		{
			m_skyboxMaterial->Destroy ();
			delete m_skyboxMaterial;
			m_skyboxMaterial = nullptr;
		}

		m_shadowMapShader = nullptr;
		m_omnidirectionalShadowMapShader = nullptr;

		m_defaultForwardPipeline.Release ();
		m_defaultLinePipeline.Release ();

		m_swapChainBuffer = nullptr;
		m_swapChainRenderTarget = nullptr;
		m_screenDepthStencilBuffer = nullptr;
		m_screenDepthStencil = nullptr;

		m_usingRenderTarget = nullptr;
		m_usingDepthStencil = nullptr;

		m_rsCullBack = nullptr;
		m_rsCullNone = nullptr;

		m_depthLessStencilAlways = nullptr;
		m_dssLessEqualStencilAlways = nullptr;

		m_constantBufferPerObject = nullptr;
		m_constantBufferPerCamera = nullptr;
		m_lightingConstantBuffer = nullptr;

		m_shadowMapSampler = nullptr;

		m_directionalLightShadowMap = nullptr;
		m_directionalLightShadowMapSRV = nullptr;
		m_directionalLightShadowMapDSV.clear ();

		m_pointLightShadowMap = nullptr;
		m_pointLightShadowMapSRV = nullptr;
		m_pointLightShadowMapDSV = nullptr;
		m_pointLightShadowMapRTV.clear ();

		m_spotLightShadowMap = nullptr;
		m_spotLightShadowMapSRV = nullptr;
		m_spotLightShadowMapDSV.clear ();

		m_ri->Shutdown ();
		m_ri = nullptr;

		Modular::ModuleManager::UnloadModule (m_riModulePath);
	}

	void GlobalRenderer::PresentSwapChain ()
	{
		m_ri->Present (m_settings.m_bVSyncEnabled);
	}

	bool GlobalRenderer::ResizeSwapChain (uint32 width, uint32 height, bool bFullscreen)
	{
		uint32 oldWidth = GetSwapChainWidth ();
		uint32 oldHeight = GetSwapChainHeight ();
		bool bUpdateSwapChain = (width >= 0 && height >= 0) && (width != oldWidth || height != oldHeight);

		if (bUpdateSwapChain)
		{
			bool bUsingSwapChainRenderTarget = m_usingRenderTarget == m_swapChainRenderTarget;

			if (bUsingSwapChainRenderTarget)
			{
				m_usingRenderTarget = nullptr;
			}

			m_swapChainRenderTarget = nullptr;
			m_swapChainBuffer = nullptr;

			if (m_ri->ResizeSwapChainBuffer (width, height, bFullscreen))
			{
				bool bUpdateResource = UpdateSwapChainResource ();

				if (bUsingSwapChainRenderTarget && bUpdateResource)
				{
					m_usingRenderTarget = m_swapChainRenderTarget;
				}
			}
		}

		return m_swapChainBuffer != nullptr && m_swapChainRenderTarget != nullptr;
	}

	bool GlobalRenderer::UpdateSwapChainResource ()
	{
		m_swapChainBuffer = m_ri->GetSwapChainBuffer ();

		if (m_swapChainBuffer != nullptr)
		{
			RI_Texture2D* swapChain = m_swapChainBuffer.get ();
			uint32 arraySize = m_swapChainBuffer->m_arraySize;

			m_swapChainRenderTarget = m_ri->CreateRenderTargetView (swapChain, 0, arraySize, 0);
		}

		return m_swapChainBuffer != nullptr && m_swapChainRenderTarget != nullptr;
	}

	RI_RenderTargetView* GlobalRenderer::GetSwapChainRenderTarget () const
	{
		return m_swapChainRenderTarget.get ();
	}

	uint32 GlobalRenderer::GetSwapChainWidth () const
	{
		return m_swapChainBuffer != nullptr ? m_swapChainBuffer->m_width : 0;
	}

	uint32 GlobalRenderer::GetSwapChainHeight () const
	{
		return m_swapChainBuffer != nullptr ? m_swapChainBuffer->m_height : 0;
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
					if (m_swapChainRenderTarget == m_usingRenderTarget)
					{
						m_settings.m_renderWidth = width;
						m_settings.m_renderHeight = height;
					}

					m_settings.m_bFullScreenEnabled = bFullscreen;
				}
			}
		}
	}

	bool GlobalRenderer::UpdateScreenDepthStencilResource ()
	{
		uint32 width = m_swapChainBuffer->m_width;
		uint32 height = m_swapChainBuffer->m_height;
		uint32 mipmapCount = m_swapChainBuffer->m_mipmapCount;
		uint32 arraySize = m_swapChainBuffer->m_arraySize;

		bool bUsingScreenDepthStencil = m_usingDepthStencil == m_screenDepthStencil;

		if (bUsingScreenDepthStencil)
		{
			m_usingDepthStencil = nullptr;
		}

		m_screenDepthStencilBuffer = m_ri->CreateTexture2D (width, height, mipmapCount, arraySize, ERenderingResourceFormat::D24_UNorm_S8_UInt, nullptr, nullptr, false, false, false, true);
		m_screenDepthStencil = m_ri->CreateDepthStencilView (m_screenDepthStencilBuffer.get (), 0, arraySize, 0);

		if (bUsingScreenDepthStencil && m_screenDepthStencil != nullptr)
		{
			m_usingDepthStencil = m_screenDepthStencil;
		}

		return m_screenDepthStencilBuffer != nullptr && m_screenDepthStencil != nullptr;
	}

	RI_DepthStencilView* GlobalRenderer::GetScreenDepthStencil () const
	{
		return m_screenDepthStencil.get ();
	}

	void GlobalRenderer::BindRenderTargetAndDepthStencil ()
	{
		RI_RenderTargetView* renderTarget = m_usingRenderTarget.get ();
		RI_DepthStencilView* depthStencil = m_usingDepthStencil.get ();

		m_ri->BindRenderTargetViewAndDepthStencilView (renderTarget, depthStencil);
	}

	void GlobalRenderer::ClearRenderTargetAndDepthStencil (Vector4 color, float depth, uint8 stencil)
	{
		RI_RenderTargetView* renderTarget = m_usingRenderTarget.get ();
		RI_DepthStencilView* depthStencil = m_usingDepthStencil.get ();

		m_ri->ClearRenderTarget (renderTarget, color.m_x, color.m_y, color.m_z, color.m_w);
		m_ri->ClearDepthStencil (depthStencil, depth, stencil);
	}

	RI_RenderTargetView* GlobalRenderer::GetRenderTarget () const
	{
		return m_usingRenderTarget.get ();
	}

	RI_DepthStencilView* GlobalRenderer::GetDepthStencil () const
	{
		return m_usingDepthStencil.get ();
	}

	void GlobalRenderer::SetRenderTargetAndDepthStencilAsDefault ()
	{
		SetRenderTarget (m_swapChainRenderTarget);
		SetDepthStencil (m_screenDepthStencil);
	}

	void GlobalRenderer::SetRenderTarget (RenderingResourcePtr<RI_RenderTargetView> renderTarget)
	{
		m_usingRenderTarget = renderTarget;
	}

	void GlobalRenderer::SetDepthStencil (RenderingResourcePtr<RI_DepthStencilView> depthStencil)
	{
		m_usingDepthStencil = depthStencil;
	}

	Vector2 GlobalRenderer::GetRenderSize () const
	{
		float width = static_cast<float> (m_settings.m_renderWidth);
		float height = static_cast<float> (m_settings.m_renderHeight);

		return Vector2 (width, height);
	}

	void GlobalRenderer::SetRenderSize (Vector2 size)
	{
		m_settings.m_renderWidth = static_cast<uint32> (size.m_x);
		m_settings.m_renderHeight = static_cast<uint32> (size.m_y);
	}

	Vector2 GlobalRenderer::GetViewportSize () const
	{
		return m_viewportSize;
	}

	Vector2 GlobalRenderer::GetViewportTopLeft () const
	{
		return m_viewportTopLeft;
	}

	void GlobalRenderer::SetViewport (Vector2 size, Vector2 topLeft)
	{
		if (m_ri->SetViewport (size.m_x, size.m_y, topLeft.m_x, topLeft.m_y))
		{
			m_viewportSize = size;
			m_viewportTopLeft = topLeft;
		}
	}

	void GlobalRenderer::ActivateShadowMapShader (int32 lightType)
	{
		ELightType type = static_cast<ELightType> (lightType);

		if (type == ELightType::Directional || type == ELightType::Spot)
		{
			m_ri->ActivateVertexShader (m_shadowMapShader->GetVertexShader ());
			m_ri->ActivatePixelShader (nullptr);
			m_ri->ActivateHullShader (nullptr);
			m_ri->ActivateDomainShader (nullptr);
			m_ri->ActivateGeometryShader (nullptr);

			m_ri->SetInputLayout (m_shadowMapShader->GetInputLayout ());

			m_ri->BindVertexShaderConstantBuffer (m_constantBufferPerCamera.get (), m_shadowMapShader->GetShaderResourceBinding (EShaderStage::Vertex, "CBCamera").m_bindIndex);
			m_ri->BindVertexShaderConstantBuffer (m_constantBufferPerObject.get (), m_shadowMapShader->GetShaderResourceBinding (EShaderStage::Vertex, "CBObject").m_bindIndex);
		}
		else if (type == ELightType::Point)
		{
			m_ri->ActivateVertexShader (m_omnidirectionalShadowMapShader->GetVertexShader ());
			m_ri->ActivatePixelShader (m_omnidirectionalShadowMapShader->GetPixelShader ());
			m_ri->ActivateHullShader (nullptr);
			m_ri->ActivateDomainShader (nullptr);
			m_ri->ActivateGeometryShader (nullptr);

			m_ri->SetInputLayout (m_omnidirectionalShadowMapShader->GetInputLayout ());

			m_ri->BindVertexShaderConstantBuffer (m_constantBufferPerCamera.get (), m_omnidirectionalShadowMapShader->GetShaderResourceBinding (EShaderStage::Vertex, "CBCamera").m_bindIndex);
			m_ri->BindVertexShaderConstantBuffer (m_constantBufferPerObject.get (), m_omnidirectionalShadowMapShader->GetShaderResourceBinding (EShaderStage::Vertex, "CBObject").m_bindIndex);

			m_ri->BindPixelShaderConstantBuffer (m_constantBufferPerCamera.get (), m_omnidirectionalShadowMapShader->GetShaderResourceBinding (EShaderStage::Pixel, "CBCamera").m_bindIndex);
		}
	}

	void GlobalRenderer::BindShadowRenderTarget (int32 lightType, uint32 renderTargetIndex)
	{
		if (static_cast<ELightType> (lightType) == ELightType::Directional)
		{
			if (renderTargetIndex >= m_directionalLightShadowMapDSV.size ())
			{
				return;
			}

			m_ri->BindPixelShaderResource (nullptr, m_directionalLightShadowMapLastSlot);

			auto* shadowDepthStencil = m_directionalLightShadowMapDSV.at (renderTargetIndex).get ();

			m_ri->BindRenderTargetViewAndDepthStencilView (nullptr, shadowDepthStencil);
			m_ri->ClearDepthStencil (shadowDepthStencil, 1.0f, 0);

			m_ri->SetViewport (2048, 2048, 0, 0);
		}
		else if (static_cast<ELightType> (lightType) == ELightType::Point)
		{
			if (renderTargetIndex >= m_pointLightShadowMapRTV.size ())
			{
				return;
			}

			m_ri->BindPixelShaderResource (nullptr, m_pointLightShadowMapLastSlot);
			auto* renderTarget = m_pointLightShadowMapRTV.at (renderTargetIndex).get ();

			m_ri->BindRenderTargetViewAndDepthStencilView (renderTarget, m_pointLightShadowMapDSV.get ());
			m_ri->ClearRenderTarget (renderTarget, 0.0f, 0.0f, 0.0f, 0.0f);
			m_ri->ClearDepthStencil (m_pointLightShadowMapDSV.get (), 1.0f, 0);

			m_ri->SetViewport (1024, 1024, 0, 0);
		}
		else if (static_cast<ELightType> (lightType) == ELightType::Spot)
		{
			if (renderTargetIndex >= m_spotLightShadowMapDSV.size ())
			{
				return;
			}

			m_ri->BindPixelShaderResource (nullptr, m_spotLightShadowMapLastSlot);

			auto* shadowDepthStencil = m_spotLightShadowMapDSV.at (renderTargetIndex).get ();

			m_ri->BindRenderTargetViewAndDepthStencilView (nullptr, shadowDepthStencil);
			m_ri->ClearDepthStencil (shadowDepthStencil, 1.0f, 0);

			m_ri->SetViewport (1024, 1024, 0, 0);
		}
	}

	void GlobalRenderer::BindRenderPipeline (RenderPipeline* renedrPipeline)
	{
		m_renderPipeline = renedrPipeline;
	}

	DefaultForwardRenderPipeline* GlobalRenderer::GetDefaultForwardRenderPipeline ()
	{
		return &m_defaultForwardPipeline;
	}

	DefaultLineRenderPipeline* GlobalRenderer::GetDefaultLineRenderPipeline ()
	{
		return &m_defaultLinePipeline;
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

		m_ri->SetInputLayout (shader->GetInputLayout ());
		m_ri->ActivateVertexShader (shader->GetVertexShader ());
		m_ri->ActivatePixelShader (shader->GetPixelShader ());
		m_ri->ActivateHullShader (shader->GetHullShader ());
		m_ri->ActivateDomainShader (shader->GetDomainShader ());
		m_ri->ActivateGeometryShader (shader->GetGeometryShader ());

		BindGlobalShaderConstantBuffer (shader, "CBObject");
		BindGlobalShaderConstantBuffer (shader, "CBCamera");
		BindGlobalShaderConstantBuffer (shader, "CBLighting");

		// Vertex bindings
		if (shader->IsShaderResourceExist (EShaderStage::Vertex, "$Globals"))
		{
			m_ri->BindVertexShaderConstantBuffer (shader->GetVertexShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Vertex, "$Globals").m_bindIndex);
		}

		// Pixel bindings
		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "$Globals"))
		{
			m_ri->BindPixelShaderConstantBuffer (shader->GetPixelShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Pixel, "$Globals").m_bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_DirectionalLightShadowMap"))
		{
			m_directionalLightShadowMapLastSlot = shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_DirectionalLightShadowMap").m_bindIndex;
			m_ri->BindPixelShaderResource (m_directionalLightShadowMapSRV.get (), m_directionalLightShadowMapLastSlot);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_PointLightShadowMap"))
		{
			m_pointLightShadowMapLastSlot = shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_PointLightShadowMap").m_bindIndex;
			m_ri->BindPixelShaderResource (m_pointLightShadowMapSRV.get (), m_pointLightShadowMapLastSlot);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_SpotLightShadowMap"))
		{
			m_spotLightShadowMapLastSlot = shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_SpotLightShadowMap").m_bindIndex;
			m_ri->BindPixelShaderResource (m_spotLightShadowMapSRV.get (), m_spotLightShadowMapLastSlot);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_ShadowMapSampler"))
		{
			m_ri->BindPixelShaderSampler (m_shadowMapSampler.get (), shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_ShadowMapSampler").m_bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_EnvironmentTexture"))
		{
			m_ri->BindPixelShaderResource (m_skyboxMaterial->GetTextureMap ().at ("Skybox")->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_EnvironmentTexture").m_bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, "g_EnvironmentTextureSampler"))
		{
			m_ri->BindPixelShaderSampler (m_skyboxMaterial->GetTextureMap ().at ("Skybox")->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Pixel, "g_EnvironmentTextureSampler").m_bindIndex);
		}

		// Hull bindings

		if (shader->IsShaderResourceExist (EShaderStage::Hull, "$Globals"))
		{
			m_ri->BindHullShaderConstantBuffer (shader->GetHullShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Hull, "$Globals").m_bindIndex);
		}

		// Domain bindings
		if (shader->IsShaderResourceExist (EShaderStage::Domain, "CBObject"))
		{
			m_ri->BindDomainShaderConstantBuffer (m_constantBufferPerObject.get (), shader->GetShaderResourceBinding (EShaderStage::Domain, "CBObject").m_bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Domain, "CBCamera"))
		{
			m_ri->BindDomainShaderConstantBuffer (m_constantBufferPerCamera.get (), shader->GetShaderResourceBinding (EShaderStage::Domain, "CBCamera").m_bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Domain, "CBLighting"))
		{
			m_ri->BindDomainShaderConstantBuffer (m_lightingConstantBuffer.get (), shader->GetShaderResourceBinding (EShaderStage::Domain, "CBLighting").m_bindIndex);
		};

		if (shader->IsShaderResourceExist (EShaderStage::Domain, "$Globals"))
		{
			m_ri->BindDomainShaderConstantBuffer (shader->GetDomainShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Domain, "$Globals").m_bindIndex);
		}

		// Geometry bindings
		if (shader->IsShaderResourceExist (EShaderStage::Geometry, "CBObject"))
		{
			m_ri->BindGeometryShaderConstantBuffer (m_constantBufferPerObject.get (), shader->GetShaderResourceBinding (EShaderStage::Geometry, "CBObject").m_bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Geometry, "CBCamera"))
		{
			m_ri->BindGeometryShaderConstantBuffer (m_constantBufferPerCamera.get (), shader->GetShaderResourceBinding (EShaderStage::Geometry, "CBCamera").m_bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Geometry, "CBLighting"))
		{
			m_ri->BindGeometryShaderConstantBuffer (m_lightingConstantBuffer.get (), shader->GetShaderResourceBinding (EShaderStage::Geometry, "CBLighting").m_bindIndex);
		};

		if (shader->IsShaderResourceExist (EShaderStage::Geometry, "$Globals"))
		{
			m_ri->BindGeometryShaderConstantBuffer (shader->GetDomainShaderParameterBuffer (), shader->GetShaderResourceBinding (EShaderStage::Geometry, "$Globals").m_bindIndex);
		}


		for (const auto& textureBinding : material->GetTextureMap ())
		{
			const std::string& name = textureBinding.first;
			Texture* texture = textureBinding.second;

			if (texture != nullptr)
			{
				if (shader->IsShaderResourceExist (EShaderStage::Vertex, name))
				{
					m_ri->BindVertexShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Vertex, name).m_bindIndex);
					m_ri->BindVertexShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Vertex, name + "Sampler").m_bindIndex);
				}

				if (shader->IsShaderResourceExist (EShaderStage::Pixel, name))
				{
					m_ri->BindPixelShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Pixel, name).m_bindIndex);
					m_ri->BindPixelShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Pixel, name + "Sampler").m_bindIndex);
				}

				if (shader->IsShaderResourceExist (EShaderStage::Hull, name))
				{
					m_ri->BindHullShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Hull, name).m_bindIndex);
					m_ri->BindHullShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Hull, name + "Sampler").m_bindIndex);
				}

				if (shader->IsShaderResourceExist (EShaderStage::Domain, name))
				{
					m_ri->BindDomainShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Domain, name).m_bindIndex);
					m_ri->BindDomainShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Domain, name + "Sampler").m_bindIndex);
				}

				if (shader->IsShaderResourceExist (EShaderStage::Geometry, name))
				{
					m_ri->BindGeometryShaderResource (texture->GetSRV (), shader->GetShaderResourceBinding (EShaderStage::Geometry, name).m_bindIndex);
					m_ri->BindGeometryShaderSampler (texture->GetSampler (), shader->GetShaderResourceBinding (EShaderStage::Geometry, name + "Sampler").m_bindIndex);
				}
			}
		}
	}

	void GlobalRenderer::BindGlobalShaderConstantBuffer (Shader* shader, const std::string& name)
	{
		if (shader == nullptr)
		{
			return;
		}

		RI_ShaderConstantBuffer* buffer = nullptr;
		uint32 bindIndex = 0;

		if (name == "CBObject")
		{
			buffer = m_constantBufferPerObject.get ();
		}

		if (name == "CBCamera")
		{
			buffer = m_constantBufferPerCamera.get ();
		}

		if (name == "CBLighting")
		{
			buffer = m_lightingConstantBuffer.get ();
		};

		if (buffer == nullptr)
		{
			return;
		}

		if (shader->IsShaderResourceExist (EShaderStage::Vertex, name))
		{

			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Vertex, name).m_bindIndex;
			m_ri->BindVertexShaderConstantBuffer (buffer, bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Pixel, name))
		{

			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Pixel, name).m_bindIndex;
			m_ri->BindPixelShaderConstantBuffer (buffer, bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Hull, name))
		{

			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Hull, name).m_bindIndex;
			m_ri->BindHullShaderConstantBuffer (buffer, bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Domain, name))
		{

			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Domain, name).m_bindIndex;
			m_ri->BindDomainShaderConstantBuffer (buffer, bindIndex);
		}

		if (shader->IsShaderResourceExist (EShaderStage::Geometry, name))
		{

			bindIndex = shader->GetShaderResourceBinding (EShaderStage::Geometry, name).m_bindIndex;
			m_ri->BindGeometryShaderConstantBuffer (buffer, bindIndex);
		}
	}

	void GlobalRenderer::SetGlobalShaderConstantBuffer (const std::string& name, const void* bufferData)
	{
		if (name == "CBObject")
		{
			m_ri->UpdateShaderConstantBuffer (m_constantBufferPerObject.get (), bufferData);
		}
		else if (name == "CBCamera")
		{
			m_ri->UpdateShaderConstantBuffer (m_constantBufferPerCamera.get (), bufferData);
		}
		else if (name == "CBLighting")
		{
			m_ri->UpdateShaderConstantBuffer (m_lightingConstantBuffer.get (), bufferData);
		};
	}

	void GlobalRenderer::DrawVertices (RI_VertexBuffer* vertexBuffer, RI_IndexBuffer* indexBuffer)
	{
		if (indexBuffer == nullptr)
		{
			m_ri->Draw (vertexBuffer);
		}
		else
		{
			m_ri->DrawIndexed (vertexBuffer, indexBuffer);
		}
	}

	void GlobalRenderer::RenderScene (Scene* scene)
	{
		if (scene == nullptr)
		{
			return;
		}

		auto& gameObjects = scene->GetGameObjects ();
		std::vector<Camera*> cameras;
		std::vector<Renderer*> renderers;
		std::vector<Light*> lights;

		for (auto& gameObject : gameObjects)
		{
			if (gameObject->IsActive () == false)
			{
				continue;
			}

			auto cameraComponents = gameObject->GetComponents<Camera> ();
			cameras.insert (cameras.end (), cameraComponents.begin (), cameraComponents.end ());

			auto rendererComponents = gameObject->GetComponents<Renderer> ();
			renderers.insert (renderers.end (), rendererComponents.begin (), rendererComponents.end ());

			auto lightComponents = gameObject->GetComponents<Light> ();
			lights.insert (lights.end (), lightComponents.begin (), lightComponents.end ());
		}

		m_renderPipeline->Start (cameras, renderers, lights);
		m_renderPipeline->End ();
	}

	CullData GlobalRenderer::Cull (Camera* camera, const std::vector<Renderer*>& renderers)
	{
		CullData cullData;
		cullData.m_camera = camera;
		cullData.m_visibleRenderers = renderers;

		return cullData;
	}

	void GlobalRenderer::DrawSkybox ()
	{
		if (Camera::Main == nullptr)
		{
			return;
		}

		m_ri->SetRasterizerState (m_rsCullNone.get ());
		m_ri->SetDepthStencilState (m_dssLessEqualStencilAlways.get ());

		m_ri->ActivateVertexShader (m_skyboxMaterial->GetShader ()->GetVertexShader ());
		m_ri->ActivatePixelShader (m_skyboxMaterial->GetShader ()->GetPixelShader ());

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

		g_renderer.SetGlobalShaderConstantBuffer ("CBCamera", &cameraConstantBuffer);

		auto* skyMesh = g_assetManager.FindAsset<Mesh> (L"Assets/sphere.obj");

		m_ri->SetInputLayout (m_skyboxMaterial->GetShader ()->GetInputLayout ());
		m_ri->BindVertexShaderConstantBuffer (m_constantBufferPerCamera.get (), m_skyboxMaterial->GetShader ()->GetShaderResourceBinding (EShaderStage::Vertex, "CBCamera").m_bindIndex);
		m_ri->BindPixelShaderResource (m_skyboxMaterial->GetTextureMap ().find ("Skybox")->second->GetSRV (), m_skyboxMaterial->GetShader ()->GetShaderResourceBinding (EShaderStage::Pixel, "Skybox").m_bindIndex);
		m_ri->BindPixelShaderSampler (m_skyboxMaterial->GetTextureMap ().find ("Skybox")->second->GetSampler (), m_skyboxMaterial->GetShader ()->GetShaderResourceBinding (EShaderStage::Pixel, "SkyboxSampler").m_bindIndex);

		DrawVertices (skyMesh->GetVertexBufferResource (0), skyMesh->GetIndexBufferResource (0));

		m_ri->SetRasterizerState (m_rsCullBack.get ());
		m_ri->SetDepthStencilState (m_depthLessStencilAlways.get ());
	}

	LightData GlobalRenderer::GetLightData (uint32 index)
	{
		LightData data;

		if (index < m_maxLightCount)
		{
			data = m_lightData.at (index);
		}

		return data;
	}

	void GlobalRenderer::AddLightData (LightData data)
	{
		if (m_lightData.size () >= m_maxLightCount)
		{
			return;
		}

		m_lightData.push_back (data);
	}

	void GlobalRenderer::ResetLightData ()
	{
		m_lightData.clear ();
	}

	void GlobalRenderer::UpdateLightDataBuffer ()
	{
		LightingConstantBuffer lightingConstantBuffer;
		lightingConstantBuffer.m_lightCount = Math::Min (static_cast<int32> (m_lightData.size ()), m_maxLightCount);
		lightingConstantBuffer.m_shadowDistance = 0.0f;

		for (int32 i = 0; i < lightingConstantBuffer.m_lightCount; i++)
		{
			lightingConstantBuffer.m_data[i] = m_lightData.at (i);
		}

		SetGlobalShaderConstantBuffer ("CBLighting", &lightingConstantBuffer);
	}

	int32 GlobalRenderer::GetLightCount () const
	{
		return m_lightData.size ();
	}

	int32 GlobalRenderer::GetMaxLightCount () const
	{
		return m_maxLightCount;
	}

	PlatformRenderingInterface& GlobalRenderer::GetPlatformRenderingInterface ()
	{
		return *m_ri;
	}

	GlobalRendererSettings GlobalRenderer::GetSettings () const
	{
		return m_settings;
	}
}
