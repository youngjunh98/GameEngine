#pragma once

#include "Engine/RI/D3D11/D3D11.h"
#include "Engine/RI/RenderingInterface.h"

namespace GameEngine
{
	class D3D11RenderingInterface final : public RenderingInterface
	{
	public:
		D3D11RenderingInterface ();
		virtual ~D3D11RenderingInterface ();

		ENGINE_RI_API virtual bool Initialize (uint32 swapChainWidth, uint32 swapChainHeight, bool bFullScreen, bool bVSync, uint32 refreshRate, uint32 msaaSampleCount) override;
		ENGINE_RI_API virtual void Shutdown () override;

		ENGINE_RI_API virtual void Present (bool bVSyncEnabled) override;
		ENGINE_RI_API virtual bool ResizeSwapChainBuffer (uint32 width, uint32 height, bool bFullScreen) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_Texture2D> GetSwapChainBuffer () override;

		ENGINE_RI_API virtual bool SetViewport (float width, float height, float topLeftX, float topLeftY) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_VertexBuffer> CreateVertexBuffer (const void* const vertices, uint32 vertexCount, uint32 vertexStride) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_IndexBuffer> CreateIndexBuffer (const void* const indices, uint32 indexCount, uint32 indexStride) override;

		ENGINE_RI_API virtual void SetPrimitiveTopology (EPrimitiveTopology primitiveTopology) override;
		ENGINE_RI_API virtual void Draw (const RI_VertexBuffer* vertexBuffer) override;
		ENGINE_RI_API virtual void DrawIndexed (const RI_VertexBuffer* vertexBuffer, const RI_IndexBuffer* indexBuffer) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_ShaderConstantBuffer> CreateShaderConstantBuffer (uint32 size, const void* data) override;
		ENGINE_RI_API virtual void UpdateShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, const void* data) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_Texture1D> CreateTexture1D (uint32 width, uint32 mipmapCount, uint32 arraySize, ERenderingResourceFormat format, void** data) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_Texture2D> CreateTexture2D (uint32 width, uint32 height, uint32 mipmapCount, uint32 arraySize, ERenderingResourceFormat format, void** data, uint32* dataPitch, bool bTextureCube, bool bShaderResource, bool bRenderTarget, bool bDepthStencil) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_Texture3D> CreateTexture3D (uint32 width, uint32 height, uint32 depth, uint32 mipmapCount, ERenderingResourceFormat format, void** data, uint32* dataPitch, uint32* dataSlicePitch) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_ShaderResourceView> CreateShaderResourceView (const RI_Texture1D* texture1D, ERenderingResourceFormat format, uint32 firstMipmapIndex, uint32 arraySize, uint32 firstArrayIndex) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_ShaderResourceView> CreateShaderResourceView (const RI_Texture2D* texture2D, ERenderingResourceFormat format, uint32 firstMipmapIndex, uint32 arraySize, uint32 firstArrayIndex) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_ShaderResourceView> CreateShaderResourceView (const RI_Texture3D* texture3D, ERenderingResourceFormat format, uint32 firstMipmapIndex) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_Sampler> CreateSampler (EAddressMode addressMode, EFilterMode filterMode, uint32 anisotropicLevel, float borderColor[4]) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_VertexShader> CreateVertexShader (const void* compiledCode, uint32 compiledCodeByteSize) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_HullShader> CreateHullShader (const void* compiledCode, uint32 compiledCodeByteSize) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_DomainShader> CreateDomainShader (const void* compiledCode, uint32 compiledCodeByteSize) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_GeometryShader> CreateGeometryShader (const void* compiledCode, uint32 compiledCodeByteSize) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_PixelShader> CreatePixelShader (const void* compiledCode, uint32 compiledCodeByteSize) override;

		ENGINE_RI_API virtual void ActivateVertexShader (const RI_VertexShader* vertexShader) override;
		ENGINE_RI_API virtual void ActivateHullShader (const RI_HullShader* hullShader) override;
		ENGINE_RI_API virtual void ActivateDomainShader (const RI_DomainShader* domainShader) override;
		ENGINE_RI_API virtual void ActivateGeometryShader (const RI_GeometryShader* geometryShader) override;
		ENGINE_RI_API virtual void ActivatePixelShader (const RI_PixelShader* pixelShader) override;

		ENGINE_RI_API virtual void BindVertexShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) override;
		ENGINE_RI_API virtual void BindHullShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) override;
		ENGINE_RI_API virtual void BindDomainShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) override;
		ENGINE_RI_API virtual void BindGeometryShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) override;
		ENGINE_RI_API virtual void BindPixelShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) override;

		ENGINE_RI_API virtual void BindVertexShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) override;
		ENGINE_RI_API virtual void BindHullShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) override;
		ENGINE_RI_API virtual void BindDomainShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) override;
		ENGINE_RI_API virtual void BindGeometryShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) override;
		ENGINE_RI_API virtual void BindPixelShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) override;

		ENGINE_RI_API virtual void BindVertexShaderSampler (const RI_Sampler* sampler, uint32 index) override;
		ENGINE_RI_API virtual void BindHullShaderSampler (const RI_Sampler* sampler, uint32 index) override;
		ENGINE_RI_API virtual void BindDomainShaderSampler (const RI_Sampler* sampler, uint32 index) override;
		ENGINE_RI_API virtual void BindGeometryShaderSampler (const RI_Sampler* sampler, uint32 index) override;
		ENGINE_RI_API virtual void BindPixelShaderSampler (const RI_Sampler* sampler, uint32 index) override;

		ENGINE_RI_API virtual bool CompileShader (const std::wstring& sourcePath, const std::string& entryPoint, EShaderStage shaderStage,
			std::unique_ptr<uint8[]>& compiledCode, uint32& compiledCodeSize) override;
		ENGINE_RI_API virtual bool ReflectShaderInformation (const void* compiledCode, uint32 compiledCodeByteSize, RI_Shader* shader) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_RenderTargetView> CreateRenderTargetView (const RI_Texture2D* renderBuffer, uint32 mipmapIndex, uint32 arraySize, uint32 firstArrayIndex) override;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_DepthStencilView> CreateDepthStencilView (const RI_Texture2D* depthStencilBuffer, uint32 mipmapIndex, uint32 arraySize, uint32 firstArrayIndex) override;
		ENGINE_RI_API virtual void BindRenderTargetViewAndDepthStencilView (const RI_RenderTargetView* renderTargetView, const RI_DepthStencilView* depthStencilView) override;
		ENGINE_RI_API virtual void ClearRenderTarget (const RI_RenderTargetView* renderTargetView, float red, float green, float blue, float alpha) override;
		ENGINE_RI_API virtual void ClearDepthStencil (const RI_DepthStencilView* depthStencilView, float depth, uint8 stencil) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_RasterizerState> CreateRasterizerState (EFillMode fillMode, EFaceCulling faceCulling, EWindingOrder windingOrder) override;
		ENGINE_RI_API virtual void SetRasterizerState (const RI_RasterizerState* rasterizerState) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_DepthStencilState> CreateDepthStencilState (EComparisonFunction depthFunction, EComparisonFunction stencilFunction) override;
		ENGINE_RI_API virtual void SetDepthStencilState (const RI_DepthStencilState* depthStencilState) override;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_InputLayout> CreateInputLayout (const void* compiledCode, uint32 compiledCodeByteSize) override;
		ENGINE_RI_API virtual void SetInputLayout (const RI_InputLayout* inputLayout) override;

		ENGINE_RI_API virtual void* GetNativeDevice () override;
		ENGINE_RI_API virtual void* GetNativeContext () override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_immediateContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		
		D3D_FEATURE_LEVEL m_featureLevel;
		UINT m_msaaQuality;
	};
}
