#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")

#include "RI/D3D11/D3D11RenderingInterface.h"
#include "RI/D3D11/D3D11RenderingInterfaceResource.h"
#include "RI/D3D11/D3D11RenderingInterfaceEnum.h"
#include "Platform/Platform.h"

LINK_MODULE (D3D11RI, Engine_RI.dll, GameEngine::D3D11RenderingInterface)

namespace GameEngine
{
	D3D11RenderingInterface g_platformRI;

	bool D3D11RenderingInterface::Initialize (uint32 swapChainWidth, uint32 swapChainHeight, bool bFullScreen, bool bVSync, uint32 refreshRate, uint32 msaaSampleCount)
	{
		// Create Direct3D 11 device and context.
		const D3D_FEATURE_LEVEL targetFeatureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
		const UINT targetFeatureLevelsCount = 2;
		UINT deviceFlags = 0;

#if _DEBUG
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		if (FAILED (D3D11CreateDevice (nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, targetFeatureLevels, targetFeatureLevelsCount,
			D3D11_SDK_VERSION, m_device.ReleaseAndGetAddressOf (), &m_featureLevel, m_immediateContext.ReleaseAndGetAddressOf ())))
		{
			return false;
		}

		m_device->CheckMultisampleQualityLevels (DXGI_FORMAT_R8G8B8A8_UNORM, msaaSampleCount, &m_msaaQuality);

		//Microsoft::WRL::ComPtr<IDXGIOutput> dxgiOutput;
		//dxgiAdapter->EnumOutputs (0, &dxgiOutput);

		//UINT displayModeCount;
		//DXGI_MODE_DESC* displayModeList;

		//dxgiOutput->GetDisplayModeList (DXGI_FORMAT_R8G8B8A8_UNORM, 0, &displayModeCount, displayModeList);

		// Create swap chain.
		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		m_device.As (&dxgiDevice);

		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetAdapter (dxgiAdapter.ReleaseAndGetAddressOf ());

		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
		dxgiAdapter->GetParent (__uuidof (IDXGIFactory), &dxgiFactory);

		DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
		swapChainDesc.BufferDesc.Width = swapChainWidth;
		swapChainDesc.BufferDesc.Height = swapChainHeight;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = bVSync ? refreshRate : 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = MapRenderingResourceFormatToDXGIFormat (ERenderingResourceFormat::R8G8B8A8_UNorm);
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = static_cast<HWND> (Platform::GetGenericApplication ().GetNativeWindowHandle ());
		swapChainDesc.Windowed = bFullScreen == false;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		if (FAILED (dxgiFactory->CreateSwapChain (m_device.Get (), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf ())))
		{
			return false;
		}

		return true;
	}

	void D3D11RenderingInterface::Shutdown ()
	{
		RenderingInterface::Shutdown ();

		m_swapChainBuffer = nullptr;
		m_swapChain->SetFullscreenState (false, nullptr);
		m_swapChain.Reset ();

		m_immediateContext->Flush ();
		m_immediateContext->ClearState ();
		m_immediateContext.Reset ();
		m_device.Reset ();
	}

	void D3D11RenderingInterface::Present (bool bVSyncEnabled)
	{
		if (bVSyncEnabled)
		{
			m_swapChain->Present (1, 0);
		}
		else
		{
			m_swapChain->Present (0, 0);
		}
	}

	void D3D11RenderingInterface::ResizeSwapChainBuffer (uint32 width, uint32 height, bool bFullScreen)
	{
		if (width == 0 || height == 0)
		{
			return;
		}

		m_immediateContext->OMSetRenderTargets (0, nullptr, nullptr);
		m_immediateContext->Flush ();

		m_swapChain->ResizeBuffers (0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	}

	RenderingResourcePtr<RI_Texture2D> D3D11RenderingInterface::GetSwapChainBuffer ()
	{
		if (m_swapChainBuffer == nullptr)
		{
			auto buffer = std::make_shared<D3D11RI_Texture2D> ();

			if (FAILED (m_swapChain->GetBuffer (0, __uuidof (ID3D11Texture2D), (void **) buffer->m_resource.ReleaseAndGetAddressOf ())))
			{
				return nullptr;
			}

			D3D11_TEXTURE2D_DESC bufferDesc;
			buffer->m_resource->GetDesc (&bufferDesc);

			buffer->m_width = bufferDesc.Width;
			buffer->m_height = bufferDesc.Height;
			buffer->m_mipmapCount = bufferDesc.MipLevels;
			buffer->m_arraySize = bufferDesc.ArraySize;
			buffer->m_format = MapDXGIFormatToRenderingResourceFormat (bufferDesc.Format);
			buffer->m_bTextureCube = false;

			m_swapChainBuffer = buffer;
		}


		return m_swapChainBuffer;
	}

	void D3D11RenderingInterface::SetViewport (uint32 width, uint32 height, uint32 topLeftX, uint32 topLeftY)
	{
		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<FLOAT> (width);
		viewport.Height = static_cast<FLOAT> (height);
		viewport.TopLeftX = static_cast<FLOAT> (topLeftX);
		viewport.TopLeftY = static_cast<FLOAT> (topLeftY);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		m_immediateContext->RSSetViewports (1, &viewport);
	}

	RenderingResourcePtr<RI_VertexBuffer> D3D11RenderingInterface::CreateVertexBuffer (const void* const vertices, uint32 vertexCount, uint32 vertexStride)
	{
		if (vertices == nullptr || vertexCount <= 0 || vertexStride == 0)
		{
			return nullptr;
		}

		auto vertexBuffer = CreateRenderingResource<D3D11RI_VertexBuffer> ();
		vertexBuffer->m_bufferByteSize = vertexCount * vertexStride;
		vertexBuffer->m_elementByteSize = vertexStride;
		vertexBuffer->m_vertexCount = vertexCount;

		D3D11_BUFFER_DESC vertexBufferDesc = { };
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = vertexBuffer->m_bufferByteSize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData = { };
		vertexBufferData.pSysMem = vertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		if (FAILED (m_device->CreateBuffer (&vertexBufferDesc, &vertexBufferData, vertexBuffer->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return vertexBuffer;
	}

	RenderingResourcePtr<RI_IndexBuffer> D3D11RenderingInterface::CreateIndexBuffer (const void* const indices, uint32 indexCount, uint32 indexStride)
	{
		if (indices == nullptr || indexCount <= 0 || indexStride == 0)
		{
			return nullptr;
		}

		auto indexBuffer = CreateRenderingResource<D3D11RI_IndexBuffer> ();
		indexBuffer->m_bufferByteSize = indexCount * indexStride;
		indexBuffer->m_elementByteSize = indexStride;
		indexBuffer->m_indexCount = indexCount;

		D3D11_BUFFER_DESC indexBufferDesc = { };
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.ByteWidth = indexBuffer->m_bufferByteSize;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData = { };
		indexBufferData.pSysMem = indices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;

		if (FAILED (m_device->CreateBuffer (&indexBufferDesc, &indexBufferData, indexBuffer->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return indexBuffer;
	}

	void D3D11RenderingInterface::SetPrimitiveTopology (EPrimitiveTopology primitiveTopology)
	{
		m_immediateContext->IASetPrimitiveTopology (MapPrimitiveTopologyToD3D11PrimitiveTopology(primitiveTopology));
	}

	void D3D11RenderingInterface::Draw (const RI_VertexBuffer* vertexBuffer)
	{
		if (vertexBuffer == nullptr)
		{
			return;
		}

		auto* vertexBufferResource = static_cast<const D3D11RI_VertexBuffer&> (*vertexBuffer).m_resource.Get ();

		UINT stride = vertexBuffer->m_elementByteSize;
		UINT offset = 0;

		m_immediateContext->IASetVertexBuffers (0, 1, &vertexBufferResource, &stride, &offset);
		m_immediateContext->Draw (vertexBuffer->m_vertexCount, 0);
	}

	void D3D11RenderingInterface::DrawIndexed (const RI_VertexBuffer* vertexBuffer, const RI_IndexBuffer* indexBuffer)
	{
		if (vertexBuffer == nullptr || indexBuffer == nullptr)
		{
			return;
		}

		auto* vertexBufferResource = static_cast<const D3D11RI_VertexBuffer&> (*vertexBuffer).m_resource.Get ();
		auto* indexBufferResource = static_cast<const D3D11RI_IndexBuffer&> (*indexBuffer).m_resource.Get ();

		UINT stride = vertexBuffer->m_elementByteSize;
		UINT offset = 0;

		m_immediateContext->IASetVertexBuffers (0, 1, &vertexBufferResource, &stride, &offset);
		m_immediateContext->IASetIndexBuffer (indexBufferResource, DXGI_FORMAT_R32_UINT, 0);
		m_immediateContext->DrawIndexed (indexBuffer->m_indexCount, 0, 0);
	}

	RenderingResourcePtr<RI_ShaderConstantBuffer> D3D11RenderingInterface::CreateShaderConstantBuffer (uint32 size, const void* data)
	{
		auto constantBuffer = CreateRenderingResource<D3D11RI_ShaderConstantBuffer> ();
		constantBuffer->m_bufferByteSize = size;

		D3D11_BUFFER_DESC constantBufferDesc = { };
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.ByteWidth = size;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA constantBufferData = { };
		constantBufferData.pSysMem = data;

		D3D11_SUBRESOURCE_DATA* cbDataPointer = (data == nullptr) ? nullptr : &constantBufferData;

		if (FAILED (m_device->CreateBuffer (&constantBufferDesc, cbDataPointer, constantBuffer->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return constantBuffer;
	}

	void D3D11RenderingInterface::UpdateShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, const void* data)
	{
		if (constantBuffer == nullptr)
		{
			return;
		}

		auto* constantBufferResource = static_cast<const D3D11RI_ShaderConstantBuffer&> (*constantBuffer).m_resource.Get ();
		D3D11_MAPPED_SUBRESOURCE constantBufferData;

		if (FAILED (m_immediateContext->Map (constantBufferResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantBufferData)))
		{
			return;
		}

		auto* destination = static_cast<uint8*> (constantBufferData.pData);
		auto* source = static_cast<const uint8*> (data);

		for (uint32 i = 0; i < constantBuffer->m_bufferByteSize; i++)
		{
			destination[i] = source[i];
		}

		m_immediateContext->Unmap (constantBufferResource, 0);
	}

	RenderingResourcePtr<RI_Texture1D> D3D11RenderingInterface::CreateTexture1D (uint32 width, uint32 mipmapCount, uint32 arraySize, ERenderingResourceFormat format, void** data)
	{
		auto texture = CreateRenderingResource<D3D11RI_Texture1D> ();
		texture->m_width = width;
		texture->m_mipmapCount = mipmapCount;
		texture->m_arraySize = arraySize;
		texture->m_format = format;

		std::unique_ptr<D3D11_SUBRESOURCE_DATA[]> subresources;
		
		if (data != nullptr)
		{
			uint32 subresourceCount = mipmapCount * arraySize;
			subresources = std::make_unique<D3D11_SUBRESOURCE_DATA[]> (subresourceCount);

			for (uint32 i = 0; i < subresourceCount; i++)
			{
				subresources[i].pSysMem = data[i];
			}
		}

		D3D11_TEXTURE1D_DESC desc;
		desc.Width = width;
		desc.MipLevels = mipmapCount;
		desc.ArraySize = arraySize;
		desc.Format = MapRenderingResourceFormatToDXGIFormat (format);
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		if (FAILED (m_device->CreateTexture1D (&desc, subresources.get (), texture->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return texture;
	}

	RenderingResourcePtr<RI_Texture2D> D3D11RenderingInterface::CreateTexture2D (uint32 width, uint32 height, uint32 mipmapCount, uint32 arraySize, ERenderingResourceFormat format, void** data, uint32* dataPitch, bool bTextureCube, bool bShaderResource, bool bRenderTarget, bool bDepthStencil)
	{
		auto texture = CreateRenderingResource<D3D11RI_Texture2D> ();
		texture->m_width = width;
		texture->m_height = height;
		texture->m_mipmapCount = mipmapCount;
		texture->m_arraySize = arraySize;
		texture->m_format = format;
		texture->m_bTextureCube = bTextureCube;

		std::unique_ptr<D3D11_SUBRESOURCE_DATA[]> subresources;
		
		if (data != nullptr && dataPitch != nullptr)
		{
			uint32 subresourceCount = mipmapCount * arraySize;
			subresources = std::make_unique<D3D11_SUBRESOURCE_DATA[]> (subresourceCount);

			for (uint32 i = 0; i < subresourceCount; i++)
			{
				subresources[i].pSysMem = data[i];
				subresources[i].SysMemPitch = dataPitch[i];
			}
		}

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = mipmapCount;
		desc.ArraySize = arraySize;
		desc.Format = MapRenderingResourceFormatToDXGIFormat (format);
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		if (bTextureCube)
		{
			desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
		}

		if (bShaderResource)
		{
			desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}

		if (bRenderTarget)
		{
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		}

		if (bDepthStencil)
		{
			desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		}

		if (FAILED (m_device->CreateTexture2D (&desc, subresources.get (), texture->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return texture;
	}

	RenderingResourcePtr<RI_Texture3D> D3D11RenderingInterface::CreateTexture3D (uint32 width, uint32 height, uint32 depth, uint32 mipmapCount, ERenderingResourceFormat format, void** data, uint32* dataPitch, uint32* dataSlicePitch)
	{
		auto texture = CreateRenderingResource<D3D11RI_Texture3D> ();
		texture->m_width = width;
		texture->m_height = height;
		texture->m_depth = depth;
		texture->m_mipmapCount = mipmapCount;
		texture->m_arraySize = 1;
		texture->m_format = format;

		std::unique_ptr<D3D11_SUBRESOURCE_DATA[]> subresources;
		
		if (data != nullptr && dataPitch != nullptr && dataSlicePitch != nullptr)
		{
			subresources = std::make_unique<D3D11_SUBRESOURCE_DATA[]> (mipmapCount);

			for (uint32 i = 0; i < mipmapCount; i++)
			{
				subresources[i].pSysMem = data[i];
				subresources[i].SysMemPitch = dataPitch[i];
				subresources[i].SysMemSlicePitch = dataSlicePitch[i];
			}
		}

		D3D11_TEXTURE3D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.Depth = depth;
		desc.MipLevels = mipmapCount;
		desc.Format = MapRenderingResourceFormatToDXGIFormat (format);
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		if (FAILED (m_device->CreateTexture3D (&desc, subresources.get (), texture->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return texture;
	}

	RenderingResourcePtr<RI_ShaderResourceView> D3D11RenderingInterface::CreateShaderResourceView (const RI_Texture1D* texture1D, ERenderingResourceFormat format, uint32 firstMipmapIndex, uint32 arraySize, uint32 firstArrayIndex)
	{
		if (texture1D == nullptr)
		{
			return nullptr;
		}

		auto srv = CreateRenderingResource<D3D11RI_ShaderResourceView> ();
		auto* texture1DResource = static_cast<const D3D11RI_Texture1D&> (*texture1D).m_resource.Get ();

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = MapRenderingResourceFormatToDXGIFormat (format);

		bool bArray = texture1D->m_arraySize > 1;
		uint32 mipLevel = texture1D->m_mipmapCount - firstMipmapIndex;

		if (bArray)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
			srvDesc.Texture1DArray.MostDetailedMip = firstMipmapIndex;
			srvDesc.Texture1DArray.MipLevels = mipLevel;
			srvDesc.Texture1DArray.FirstArraySlice = firstArrayIndex;
			srvDesc.Texture1DArray.ArraySize = arraySize;
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			srvDesc.Texture1D.MostDetailedMip = firstMipmapIndex;
			srvDesc.Texture1D.MipLevels = mipLevel;
		}

		if (FAILED (m_device->CreateShaderResourceView (texture1DResource, &srvDesc, srv->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return srv;
	}

	RenderingResourcePtr<RI_ShaderResourceView> D3D11RenderingInterface::CreateShaderResourceView (const RI_Texture2D* texture2D, ERenderingResourceFormat format, uint32 firstMipmapIndex, uint32 arraySize, uint32 firstArrayIndex)
	{
		if (texture2D == nullptr)
		{
			return nullptr;
		}

		auto srv = CreateRenderingResource<D3D11RI_ShaderResourceView> ();
		auto* texture2DResource = static_cast<const D3D11RI_Texture2D&> (*texture2D).m_resource.Get ();

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = MapRenderingResourceFormatToDXGIFormat (format);

		bool bArray = texture2D->m_bTextureCube ? (texture2D->m_arraySize > 6) : (texture2D->m_arraySize > 1);
		uint32 mipLevel = texture2D->m_mipmapCount - firstMipmapIndex;

		if (texture2D->m_bTextureCube)
		{
			if (bArray)
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
				srvDesc.TextureCubeArray.MostDetailedMip = firstMipmapIndex;
				srvDesc.TextureCubeArray.MipLevels = mipLevel;
				srvDesc.TextureCubeArray.First2DArrayFace = firstArrayIndex * 6;
				srvDesc.TextureCubeArray.NumCubes = arraySize;
			}
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				srvDesc.TextureCube.MostDetailedMip = firstMipmapIndex;
				srvDesc.TextureCube.MipLevels = mipLevel;
			}
		}
		else
		{
			if (bArray)
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture2DArray.MostDetailedMip = firstMipmapIndex;
				srvDesc.Texture2DArray.MipLevels = mipLevel;
				srvDesc.Texture2DArray.FirstArraySlice = firstArrayIndex;
				srvDesc.Texture2DArray.ArraySize = arraySize;
			}
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = firstMipmapIndex;
				srvDesc.Texture2D.MipLevels = mipLevel;
			}
		}

		if (FAILED (m_device->CreateShaderResourceView (texture2DResource, &srvDesc, srv->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return srv;
	}

	RenderingResourcePtr<RI_ShaderResourceView> D3D11RenderingInterface::CreateShaderResourceView (const RI_Texture3D* texture3D, ERenderingResourceFormat format, uint32 firstMipmapIndex)
	{
		if (texture3D == nullptr)
		{
			return nullptr;
		}

		auto srv = CreateRenderingResource<D3D11RI_ShaderResourceView> ();
		auto* texture3DResource = static_cast<const D3D11RI_Texture3D&> (*texture3D).m_resource.Get ();

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = MapRenderingResourceFormatToDXGIFormat (format);
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		srvDesc.Texture3D.MostDetailedMip = firstMipmapIndex;
		srvDesc.Texture3D.MipLevels = texture3D->m_mipmapCount - firstMipmapIndex;

		if (FAILED (m_device->CreateShaderResourceView (texture3DResource, &srvDesc, srv->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return srv;
	}

	RenderingResourcePtr<RI_Sampler> D3D11RenderingInterface::CreateSampler (EAddressMode addressMode, EFilterMode filterMode, uint32 anisotropicLevel, float borderColor[4])
	{
		auto sampler = CreateRenderingResource<D3D11RI_Sampler> ();
		sampler->m_addressMode = addressMode;
		sampler->m_filterMode = filterMode;
		sampler->m_anisotropicLevel = anisotropicLevel;

		D3D11_SAMPLER_DESC samplerDesc = { };
		samplerDesc.Filter = MapFilterModeToD3D11Filter (filterMode);
		samplerDesc.AddressU = MapAddressModeToD3D11TextureAddressMode (addressMode);
		samplerDesc.AddressV = MapAddressModeToD3D11TextureAddressMode (addressMode);
		samplerDesc.AddressW = MapAddressModeToD3D11TextureAddressMode (addressMode);
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = anisotropicLevel;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = borderColor[0];
		samplerDesc.BorderColor[1] = borderColor[1];
		samplerDesc.BorderColor[2] = borderColor[2];
		samplerDesc.BorderColor[3] = borderColor[3];
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (FAILED (m_device->CreateSamplerState (&samplerDesc, sampler->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return sampler;
	}

	RenderingResourcePtr<RI_VertexShader> D3D11RenderingInterface::CreateVertexShader (const void* compiledCode, uint32 compiledCodeByteSize)
	{
		if (compiledCode == nullptr || compiledCodeByteSize <= 0)
		{
			return nullptr;
		}

		auto vertexShader = CreateRenderingResource<D3D11RI_VertexShader> ();

		if (FAILED (m_device->CreateVertexShader (compiledCode, compiledCodeByteSize, nullptr, vertexShader->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}
		
		return vertexShader;
	}

	RenderingResourcePtr<RI_HullShader> D3D11RenderingInterface::CreateHullShader (const void* compiledCode, uint32 compiledCodeByteSize)
	{
		if (compiledCode == nullptr || compiledCodeByteSize == 0)
		{
			return false;
		}

		auto hullShader = CreateRenderingResource<D3D11RI_HullShader> ();

		if (FAILED (m_device->CreateHullShader (compiledCode, compiledCodeByteSize, nullptr, hullShader->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return hullShader;
	}

	RenderingResourcePtr<RI_DomainShader> D3D11RenderingInterface::CreateDomainShader (const void* compiledCode, uint32 compiledCodeByteSize)
	{
		if (compiledCode == nullptr || compiledCodeByteSize == 0)
		{
			return false;
		}

		auto domainShader = CreateRenderingResource<D3D11RI_DomainShader> ();

		if (FAILED (m_device->CreateDomainShader (compiledCode, compiledCodeByteSize, nullptr, domainShader->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return domainShader;
	}

	RenderingResourcePtr<RI_GeometryShader> D3D11RenderingInterface::CreateGeometryShader (const void* compiledCode, uint32 compiledCodeByteSize)
	{
		if (compiledCode == nullptr || compiledCodeByteSize == 0)
		{
			return false;
		}

		auto geometryShader = CreateRenderingResource<D3D11RI_GeometryShader> ();

		if (FAILED (m_device->CreateGeometryShader (compiledCode, compiledCodeByteSize, nullptr, geometryShader->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return geometryShader;
	}

	RenderingResourcePtr<RI_PixelShader> D3D11RenderingInterface::CreatePixelShader (const void* compiledCode, uint32 compiledCodeByteSize)
	{
		if (compiledCode == nullptr || compiledCodeByteSize == 0)
		{
			return false;
		}

		auto pixelShader = CreateRenderingResource<D3D11RI_PixelShader> ();

		if (FAILED (m_device->CreatePixelShader (compiledCode, compiledCodeByteSize, nullptr, pixelShader->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return pixelShader;
	}

	void D3D11RenderingInterface::ActivateVertexShader (const RI_VertexShader* vertexShader)
	{
		if (vertexShader == nullptr)
		{
			m_immediateContext->VSSetShader (nullptr, nullptr, 0);
		}
		else
		{
			auto* vertexShaderResource = static_cast<const D3D11RI_VertexShader&> (*vertexShader).m_resource.Get ();
			m_immediateContext->VSSetShader (vertexShaderResource, nullptr, 0);
		}
	}

	void D3D11RenderingInterface::ActivateHullShader (const RI_HullShader* hullShader)
	{
		if (hullShader == nullptr)
		{
			m_immediateContext->HSSetShader (nullptr, nullptr, 0);
		}
		else
		{
			auto* hullShaderResource = static_cast<const D3D11RI_HullShader&> (*hullShader).m_resource.Get ();
			m_immediateContext->HSSetShader (hullShaderResource, nullptr, 0);
		}
	}

	void D3D11RenderingInterface::ActivateDomainShader (const RI_DomainShader* domainShader)
	{
		if (domainShader == nullptr)
		{
			m_immediateContext->DSSetShader (nullptr, nullptr, 0);
		}
		else
		{
			auto* domainShaderResource = static_cast<const D3D11RI_DomainShader&> (*domainShader).m_resource.Get ();
			m_immediateContext->DSSetShader (domainShaderResource, nullptr, 0);
		}
	}

	void D3D11RenderingInterface::ActivateGeometryShader (const RI_GeometryShader* geometryShader)
	{
		if (geometryShader == nullptr)
		{
			m_immediateContext->GSSetShader (nullptr, nullptr, 0);
		}
		else
		{
			auto* geometryShaderResource = static_cast<const D3D11RI_GeometryShader&> (*geometryShader).m_resource.Get ();
			m_immediateContext->GSSetShader (geometryShaderResource, nullptr, 0);
		}
	}

	void D3D11RenderingInterface::ActivatePixelShader (const RI_PixelShader* pixelShader)
	{
		if (pixelShader == nullptr)
		{
			m_immediateContext->PSSetShader (nullptr, nullptr, 0);
		}
		else
		{
			auto* pixelShaderResource = static_cast<const D3D11RI_PixelShader&> (*pixelShader).m_resource.Get ();
			m_immediateContext->PSSetShader (pixelShaderResource, nullptr, 0);
		}
	}

	void D3D11RenderingInterface::BindVertexShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index)
	{
		if (constantBuffer == nullptr)
		{
			ID3D11Buffer* nullBuffer[1] = { nullptr };
			m_immediateContext->VSSetConstantBuffers (index, 1, nullBuffer);
		}
		else
		{
			auto* constantBufferResource = static_cast<const D3D11RI_ShaderConstantBuffer&> (*constantBuffer).m_resource.Get ();
			m_immediateContext->VSSetConstantBuffers (index, 1, &constantBufferResource);
		}
	}

	void D3D11RenderingInterface::BindHullShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index)
	{
		if (constantBuffer == nullptr)
		{
			ID3D11Buffer* nullBuffer[1] = { nullptr };
			m_immediateContext->HSSetConstantBuffers (index, 1, nullBuffer);
		}
		else
		{
			auto* constantBufferResource = static_cast<const D3D11RI_ShaderConstantBuffer&> (*constantBuffer).m_resource.Get ();
			m_immediateContext->HSSetConstantBuffers (index, 1, &constantBufferResource);
		}
	}

	void D3D11RenderingInterface::BindDomainShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index)
	{
		if (constantBuffer == nullptr)
		{
			ID3D11Buffer* nullBuffer[1] = { nullptr };
			m_immediateContext->DSSetConstantBuffers (index, 1, nullBuffer);
		}
		else
		{
			auto* constantBufferResource = static_cast<const D3D11RI_ShaderConstantBuffer&> (*constantBuffer).m_resource.Get ();
			m_immediateContext->DSSetConstantBuffers (index, 1, &constantBufferResource);
		}
	}

	void D3D11RenderingInterface::BindGeometryShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index)
	{
		if (constantBuffer == nullptr)
		{
			ID3D11Buffer* nullBuffer[1] = { nullptr };
			m_immediateContext->GSSetConstantBuffers (index, 1, nullBuffer);
		}
		else
		{
			auto* constantBufferResource = static_cast<const D3D11RI_ShaderConstantBuffer&> (*constantBuffer).m_resource.Get ();
			m_immediateContext->GSSetConstantBuffers (index, 1, &constantBufferResource);
		}
	}

	void D3D11RenderingInterface::BindPixelShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index)
	{
		if (constantBuffer == nullptr)
		{
			ID3D11Buffer* nullBuffer[1] = { nullptr };
			m_immediateContext->PSSetConstantBuffers (index, 1, nullBuffer);
		}
		else
		{
			auto* constantBufferResource = static_cast<const D3D11RI_ShaderConstantBuffer&> (*constantBuffer).m_resource.Get ();
			m_immediateContext->PSSetConstantBuffers (index, 1, &constantBufferResource);
		}
	}

	void D3D11RenderingInterface::BindVertexShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index)
	{
		if (shaderResourceView == nullptr)
		{
			ID3D11ShaderResourceView* nullView[1] = { nullptr };
			m_immediateContext->VSSetShaderResources (index, 1, nullView);
		}
		else
		{
			auto* srvResource = static_cast<const D3D11RI_ShaderResourceView&> (*shaderResourceView).m_resource.Get ();
			m_immediateContext->VSSetShaderResources (index, 1, &srvResource);
		}
	}

	void D3D11RenderingInterface::BindHullShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index)
	{
		if (shaderResourceView == nullptr)
		{
			ID3D11ShaderResourceView* nullView[1] = { nullptr };
			m_immediateContext->HSSetShaderResources (index, 1, nullView);
		}
		else
		{
			auto* srvResource = static_cast<const D3D11RI_ShaderResourceView&> (*shaderResourceView).m_resource.Get ();
			m_immediateContext->HSSetShaderResources (index, 1, &srvResource);
		}
	}

	void D3D11RenderingInterface::BindDomainShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index)
	{
		if (shaderResourceView == nullptr)
		{
			ID3D11ShaderResourceView* nullView[1] = { nullptr };
			m_immediateContext->DSSetShaderResources (index, 1, nullView);
		}
		else
		{
			auto* srvResource = static_cast<const D3D11RI_ShaderResourceView&> (*shaderResourceView).m_resource.Get ();
			m_immediateContext->DSSetShaderResources (index, 1, &srvResource);
		}
	}

	void D3D11RenderingInterface::BindGeometryShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index)
	{
		if (shaderResourceView == nullptr)
		{
			ID3D11ShaderResourceView* nullView[1] = { nullptr };
			m_immediateContext->GSSetShaderResources (index, 1, nullView);
		}
		else
		{
			auto* srvResource = static_cast<const D3D11RI_ShaderResourceView&> (*shaderResourceView).m_resource.Get ();
			m_immediateContext->GSSetShaderResources (index, 1, &srvResource);
		}
	}

	void D3D11RenderingInterface::BindPixelShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index)
	{
		if (shaderResourceView == nullptr)
		{
			ID3D11ShaderResourceView* nullView[1] = { nullptr };
			m_immediateContext->PSSetShaderResources (index, 1, nullView);
		}
		else
		{
			auto* srvResource = static_cast<const D3D11RI_ShaderResourceView&> (*shaderResourceView).m_resource.Get ();
			m_immediateContext->PSSetShaderResources (index, 1, &srvResource);
		}
	}

	void D3D11RenderingInterface::BindVertexShaderSampler (const RI_Sampler* sampler, uint32 index)
	{
		if (sampler == nullptr)
		{
			ID3D11SamplerState* nullSampler[1] = { nullptr };
			m_immediateContext->VSSetSamplers (index, 1, nullSampler);
		}
		else
		{
			auto* samplerResource = static_cast<const D3D11RI_Sampler&> (*sampler).m_resource.Get ();
			m_immediateContext->VSSetSamplers (index, 1, &samplerResource);
		}
	}

	void D3D11RenderingInterface::BindHullShaderSampler (const RI_Sampler* sampler, uint32 index)
	{
		if (sampler == nullptr)
		{
			ID3D11SamplerState* nullSampler[1] = { nullptr };
			m_immediateContext->HSSetSamplers (index, 1, nullSampler);
		}
		else
		{
			auto* samplerResource = static_cast<const D3D11RI_Sampler&> (*sampler).m_resource.Get ();
			m_immediateContext->HSSetSamplers (index, 1, &samplerResource);
		}
	}

	void D3D11RenderingInterface::BindDomainShaderSampler (const RI_Sampler* sampler, uint32 index)
	{
		if (sampler == nullptr)
		{
			ID3D11SamplerState* nullSampler[1] = { nullptr };
			m_immediateContext->DSSetSamplers (index, 1, nullSampler);
		}
		else
		{
			auto* samplerResource = static_cast<const D3D11RI_Sampler&> (*sampler).m_resource.Get ();
			m_immediateContext->DSSetSamplers (index, 1, &samplerResource);
		}
	}

	void D3D11RenderingInterface::BindGeometryShaderSampler (const RI_Sampler* sampler, uint32 index)
	{
		if (sampler == nullptr)
		{
			ID3D11SamplerState* nullSampler[1] = { nullptr };
			m_immediateContext->GSSetSamplers (index, 1, nullSampler);
		}
		else
		{
			auto* samplerResource = static_cast<const D3D11RI_Sampler&> (*sampler).m_resource.Get ();
			m_immediateContext->GSSetSamplers (index, 1, &samplerResource);
		}
	}

	void D3D11RenderingInterface::BindPixelShaderSampler (const RI_Sampler* sampler, uint32 index)
	{
		if (sampler == nullptr)
		{
			ID3D11SamplerState* nullSampler[1] = { nullptr };
			m_immediateContext->PSSetSamplers (index, 1, nullSampler);
		}
		else
		{
			auto* samplerResource = static_cast<const D3D11RI_Sampler&> (*sampler).m_resource.Get ();
			m_immediateContext->PSSetSamplers (index, 1, &samplerResource);
		}
	}

	bool D3D11RenderingInterface::CompileShader (const std::wstring& sourcePath, const std::string& entryPoint, EShaderStage shaderStage,
		std::unique_ptr<uint8[]>& compiledCode, uint32& compiledCodeSize)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> compileResult;
		Microsoft::WRL::ComPtr<ID3DBlob> errorMessages;

		UINT shaderCompileOptions = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_PREFER_FLOW_CONTROL;
		UINT effectCompileOptions = 0;

#if _DEBUG
		shaderCompileOptions |= D3DCOMPILE_DEBUG;
#endif

		std::string shaderProfile;

		if (shaderStage == EShaderStage::Vertex)
		{
			shaderProfile = "vs_5_0";
		}
		else if (shaderStage == EShaderStage::Pixel)
		{
			shaderProfile = "ps_5_0";
		}
		else if (shaderStage == EShaderStage::Hull)
		{
			shaderProfile = "hs_5_0";
		}
		else if (shaderStage == EShaderStage::Domain)
		{
			shaderProfile = "ds_5_0";
		}
		else if (shaderStage == EShaderStage::Geometry)
		{
			shaderProfile = "gs_5_0";
		}
		else
		{
			return false;
		}

		if (FAILED (D3DCompileFromFile (sourcePath.c_str (), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str (), shaderProfile.c_str (),
			shaderCompileOptions, effectCompileOptions, compileResult.ReleaseAndGetAddressOf (), errorMessages.ReleaseAndGetAddressOf ())))
		{
			if (errorMessages)
			{
				OutputDebugStringA (static_cast<char*> (errorMessages->GetBufferPointer ()));
			}
			else
			{
				OutputDebugString (TEXT ("Can't find shader file!\n"));
			}

			return false;
		}

		const uint8* copyStart = static_cast<uint8*> (compileResult->GetBufferPointer ());
		uint32 copySize = static_cast<uint32> (compileResult->GetBufferSize ());
		
		compiledCode = std::make_unique<uint8[]> (copySize);
		compiledCodeSize = copySize;

		for (uint32 i = 0; i < copySize; i++)
		{
			compiledCode[i] = copyStart[i];
		}

		return true;
	}

	bool D3D11RenderingInterface::ReflectShaderInformation (const void* compiledCode, uint32 compiledCodeByteSize, RI_Shader* shader)
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;
		std::string globalBufferName ("$Globals");

		if (FAILED (D3DReflect (compiledCode, compiledCodeByteSize, IID_ID3D11ShaderReflection, reinterpret_cast<void**> (reflector.ReleaseAndGetAddressOf ()))))
		{
			return false;
		}

		D3D11_SHADER_DESC shaderDesc = { };
		reflector->GetDesc (&shaderDesc);

		for (UINT bindingIndex = 0; bindingIndex < shaderDesc.BoundResources; bindingIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindingDesc = { };
			reflector->GetResourceBindingDesc (bindingIndex, &bindingDesc);

			ShaderResourceBinding binding = { };
			binding.m_bindIndex = bindingDesc.BindPoint;
			binding.m_bindCount = bindingDesc.BindCount;

			if (bindingDesc.Type == D3D_SIT_CBUFFER)
			{
				ID3D11ShaderReflectionConstantBuffer* constantBuffer = reflector->GetConstantBufferByName (bindingDesc.Name);

				D3D11_SHADER_BUFFER_DESC constantBufferDesc = { };
				constantBuffer->GetDesc (&constantBufferDesc);

				binding.m_type = ShaderResourceBinding::ConstantBuffer;
				binding.m_byteSize = constantBufferDesc.Size;

				if (constantBufferDesc.Name == globalBufferName)
				{
					for (UINT variableIndex = 0; variableIndex < constantBufferDesc.Variables; variableIndex++)
					{
						ID3D11ShaderReflectionVariable* variable = constantBuffer->GetVariableByIndex (variableIndex);

						D3D11_SHADER_VARIABLE_DESC variableDesc = { };
						variable->GetDesc (&variableDesc);

						ShaderParameter parameter = { };
						parameter.m_byteSize = variableDesc.Size;
						parameter.m_byteOffset = variableDesc.StartOffset;

						shader->m_parameterMap.insert ({ variableDesc.Name, parameter });
					}
				}
			}
			else if (bindingDesc.Type == D3D_SIT_TEXTURE)
			{
				binding.m_type = ShaderResourceBinding::Texture;
			}
			else if (bindingDesc.Type == D3D_SIT_SAMPLER)
			{
				binding.m_type = ShaderResourceBinding::Sampler;
			}

			shader->m_bindingMap.insert ({ bindingDesc.Name, binding });
		}

		return true;
	}

	RenderingResourcePtr<RI_RenderTargetView> D3D11RenderingInterface::CreateRenderTargetView (const RI_Texture2D* renderBuffer, uint32 mipmapIndex, uint32 arraySize, uint32 firstArrayIndex)
	{
		if (renderBuffer == nullptr)
		{
			return nullptr;
		}

		auto rtv = CreateRenderingResource<D3D11RI_RenderTargetView> ();
		auto* bufferResource = static_cast<const D3D11RI_Texture2D&> (*renderBuffer).m_resource.Get ();

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = MapRenderingResourceFormatToDXGIFormat (renderBuffer->m_format);

		bool bArray = renderBuffer->m_arraySize > 1;

		if (bArray)
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.MipSlice = mipmapIndex;
			rtvDesc.Texture2DArray.FirstArraySlice = firstArrayIndex;
			rtvDesc.Texture2DArray.ArraySize = arraySize;
		}
		else
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = mipmapIndex;
		}

		if (FAILED (m_device->CreateRenderTargetView (bufferResource, &rtvDesc, rtv->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return rtv;
	}

	RenderingResourcePtr<RI_DepthStencilView> D3D11RenderingInterface::CreateDepthStencilView (const RI_Texture2D* depthStencilBuffer, uint32 mipmapIndex, uint32 arraySize, uint32 firstArrayIndex)
	{
		auto dsv = CreateRenderingResource<D3D11RI_DepthStencilView> ();
		auto* buffer = static_cast<const D3D11RI_Texture2D&> (*depthStencilBuffer).m_resource.Get ();

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.Flags = 0;

		bool bArray = depthStencilBuffer->m_arraySize > 1;

		if (bArray)
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			dsvDesc.Texture2DArray.MipSlice = mipmapIndex;
			dsvDesc.Texture2DArray.FirstArraySlice = firstArrayIndex;
			dsvDesc.Texture2DArray.ArraySize = arraySize;
		}
		else
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = mipmapIndex;
		}

		if (FAILED (m_device->CreateDepthStencilView (buffer, &dsvDesc, dsv->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return dsv;
	}

	void D3D11RenderingInterface::BindRenderTargetViewAndDepthStencilView (const RI_RenderTargetView* renderTargetView, const RI_DepthStencilView* depthStencilView)
	{
		ID3D11RenderTargetView* rtvResource = nullptr;
		UINT rtvCount = 0;

		ID3D11DepthStencilView* dsvResource = nullptr;

		if (renderTargetView != nullptr)
		{
			rtvResource = static_cast<const D3D11RI_RenderTargetView&> (*renderTargetView).m_resource.Get ();
			rtvCount = 1;
		}

		if (depthStencilView != nullptr)
		{
			dsvResource = static_cast<const D3D11RI_DepthStencilView&> (*depthStencilView).m_resource.Get ();
		}

		m_immediateContext->OMSetRenderTargets (rtvCount, &rtvResource, dsvResource);
	}

	void D3D11RenderingInterface::ClearRenderTarget (const RI_RenderTargetView* renderTargetView, float red, float green, float blue, float alpha)
	{
		if (renderTargetView == nullptr)
		{
			return;
		}

		auto* rtvResource = static_cast<const D3D11RI_RenderTargetView&> (*renderTargetView).m_resource.Get ();
		FLOAT color[4] = { red, green, blue, alpha };

		m_immediateContext->ClearRenderTargetView (rtvResource, color);
	}

	void D3D11RenderingInterface::ClearDepthStencil (const RI_DepthStencilView* depthStencilView, float depth, uint8 stencil)
	{
		auto* dsvResource = static_cast<const D3D11RI_DepthStencilView&> (*depthStencilView).m_resource.Get ();
		UINT flags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;

		m_immediateContext->ClearDepthStencilView (dsvResource, flags, depth, stencil);
	}

	RenderingResourcePtr<RI_RasterizerState> D3D11RenderingInterface::CreateRasterizerState (EFillMode fillMode, EFaceCulling faceCulling, EWindingOrder windingOrder)
	{
		auto rasterizerState = CreateRenderingResource<D3D11RI_RasterizerState> ();
		rasterizerState->m_fillMode = fillMode;
		rasterizerState->m_faceCulling = faceCulling;
		rasterizerState->m_windingOrder = windingOrder;

		D3D11_RASTERIZER_DESC rasterizerStateDesc = { };
		rasterizerStateDesc.FillMode = MapFillModeToD3D11FillMode (fillMode);
		rasterizerStateDesc.CullMode = MapFaceCullingToD3D11CullMode (faceCulling);
		rasterizerStateDesc.FrontCounterClockwise = (windingOrder == EWindingOrder::CCW) ? true : false;
		rasterizerStateDesc.DepthBias = 0;
		rasterizerStateDesc.DepthBiasClamp = 0.0f;
		rasterizerStateDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerStateDesc.DepthClipEnable = true;
		rasterizerStateDesc.ScissorEnable = false;
		rasterizerStateDesc.MultisampleEnable = false;
		rasterizerStateDesc.AntialiasedLineEnable = false;

		if (FAILED (m_device->CreateRasterizerState (&rasterizerStateDesc, rasterizerState->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return rasterizerState;
	}

	void D3D11RenderingInterface::SetRasterizerState (const RI_RasterizerState* rasterizerState)
	{
		if (rasterizerState == nullptr)
		{
			m_immediateContext->RSSetState (nullptr);
		}
		else
		{
			auto* rasterizerStateResource = static_cast<const D3D11RI_RasterizerState&> (*rasterizerState).m_resource.Get ();
			m_immediateContext->RSSetState (rasterizerStateResource);
		}
	}

	RenderingResourcePtr<RI_DepthStencilState> D3D11RenderingInterface::CreateDepthStencilState (EComparisonFunction depthFunction, EComparisonFunction stencilFunction)
	{
		auto depthStencilState = CreateRenderingResource<D3D11RI_DepthStencilState> ();
		depthStencilState->m_depthFunction = depthFunction;
		depthStencilState->m_stencilFunction = stencilFunction;

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = MapComparisonFunctionToD3D11ComparisonFunc (depthFunction);
		depthStencilStateDesc.StencilEnable = false;
		depthStencilStateDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		depthStencilStateDesc.FrontFace.StencilFunc = MapComparisonFunctionToD3D11ComparisonFunc (stencilFunction);
		depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFunc = MapComparisonFunctionToD3D11ComparisonFunc (stencilFunction);
		depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		if (FAILED (m_device->CreateDepthStencilState (&depthStencilStateDesc, depthStencilState->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return depthStencilState;
	}

	void D3D11RenderingInterface::SetDepthStencilState (const RI_DepthStencilState* depthStencilState)
	{
		if (depthStencilState == nullptr)
		{
			m_immediateContext->OMSetDepthStencilState (nullptr, 0);
		}
		else
		{
			auto* depthStencilStateResource = static_cast<const D3D11RI_DepthStencilState&> (*depthStencilState).m_resource.Get ();
			m_immediateContext->OMSetDepthStencilState (depthStencilStateResource, 0);
		}
	}

	RenderingResourcePtr<RI_InputLayout> D3D11RenderingInterface::CreateInputLayout (const void* compiledCode, uint32 compiledCodeByteSize)
	{
		auto layout = CreateRenderingResource<D3D11RI_InputLayout> ();

		D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		if (FAILED (m_device->CreateInputLayout (inputElementDescs, 4, compiledCode, compiledCodeByteSize, layout->m_resource.ReleaseAndGetAddressOf ())))
		{
			return nullptr;
		}

		return layout;
	}

	void D3D11RenderingInterface::SetInputLayout (const RI_InputLayout* inputLayout)
	{
		if (inputLayout == nullptr)
		{
			m_immediateContext->IASetInputLayout (nullptr);
		}
		else
		{
			auto* inputLayoutResource = static_cast<const D3D11RI_InputLayout&> (*inputLayout).m_resource.Get ();
			m_immediateContext->IASetInputLayout (inputLayoutResource);
		}
	}

	void* D3D11RenderingInterface::GetNativeDevice ()
	{
		return m_device.Get ();
	}

	void* D3D11RenderingInterface::GetNativeContext ()
	{
		return m_immediateContext.Get ();
	}
}