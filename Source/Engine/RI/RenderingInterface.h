#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Type.h"
#include "Engine/Core/Modular/Module.h"
#include "Engine/Core/File/FileSystem.h"
#include "Engine/RI/RenderingInterfaceEnum.h"
#include "Engine/RI/RenderingInterfaceResource.h"
#include "Engine/RI/RenderingInterfaceMacro.h"

namespace GameEngine
{
	enum class ERenderingInterfaceModule
	{
		D3D11
	};

	class RenderingInterface : public Modular::ModuleBase
	{
	protected:
		RenderingInterface ();
		virtual ~RenderingInterface () = 0;

	public:
		RenderingInterface (const RenderingInterface&) = delete;
		RenderingInterface& operator= (const RenderingInterface&) = delete;

		ENGINE_RI_API static bool LoadModule (ERenderingInterfaceModule renderingInterfaceModule);
		ENGINE_RI_API static void UnloadModule ();
		ENGINE_RI_API static RenderingInterface& GetModule ();

		ENGINE_RI_API virtual bool Initialize (uint32 swapChainWidth, uint32 swapChainHeight, bool bFullScreen, bool bVSync, uint32 refreshRate, uint32 msaaSampleCount) = 0;
		ENGINE_RI_API virtual void Shutdown ();

		ENGINE_RI_API virtual void Present (bool bVSyncEnabled) = 0;
		ENGINE_RI_API virtual bool ResizeSwapChainBuffer (uint32 width, uint32 height, bool bFullScreen) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_Texture2D> GetSwapChainBuffer () = 0;

		ENGINE_RI_API virtual bool SetViewport (float width, float height, float topLeftX, float topLeftY) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_VertexBuffer> CreateVertexBuffer (const void* const vertices, uint32 vertexCount, uint32 vertexStride) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_IndexBuffer> CreateIndexBuffer (const void* const indices, uint32 indexCount, uint32 indexStride) = 0;

		ENGINE_RI_API virtual void SetPrimitiveTopology (EPrimitiveTopology primitiveTopology) = 0;
		ENGINE_RI_API virtual void Draw (const RI_VertexBuffer* vertexBuffer) = 0;
		ENGINE_RI_API virtual void DrawIndexed (const RI_VertexBuffer* vertexBuffer, const RI_IndexBuffer* indexBuffer) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_ShaderConstantBuffer> CreateShaderConstantBuffer (uint32 size, const void* data) = 0;
		ENGINE_RI_API virtual void UpdateShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, const void* data) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_Texture1D> CreateTexture1D (uint32 width, uint32 mipmapCount, uint32 arraySize, ERenderingResourceFormat format, void** data) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_Texture2D> CreateTexture2D (uint32 width, uint32 height, uint32 mipmapCount,  uint32 arraySize, ERenderingResourceFormat format, void** data, uint32* dataPitch, bool bTextureCube, bool bShaderResource, bool bRenderTarget, bool bDepthStencil) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_Texture3D> CreateTexture3D (uint32 width, uint32 height, uint32 depth, uint32 mipmapCount, ERenderingResourceFormat format, void** data, uint32* dataPitch, uint32* dataSlicePitch) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_ShaderResourceView> CreateShaderResourceView (const RI_Texture1D* texture1D, ERenderingResourceFormat format, uint32 firstMipmapIndex, uint32 arraySize, uint32 firstArrayIndex) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_ShaderResourceView> CreateShaderResourceView (const RI_Texture2D* texture2D, ERenderingResourceFormat format, uint32 firstMipmapIndex, uint32 arraySize, uint32 firstArrayIndex) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_ShaderResourceView> CreateShaderResourceView (const RI_Texture3D* texture3D, ERenderingResourceFormat format, uint32 firstMipmapIndex) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_Sampler> CreateSampler (EAddressMode addressMode, EFilterMode filterMode, uint32 anisotropicLevel, float borderColor[4]) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_VertexShader> CreateVertexShader (const void* compiledCode, uint32 compiledCodeByteSize) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_HullShader> CreateHullShader (const void* compiledCode, uint32 compiledCodeByteSize) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_DomainShader> CreateDomainShader (const void* compiledCode, uint32 compiledCodeByteSize) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_GeometryShader> CreateGeometryShader (const void* compiledCode, uint32 compiledCodeByteSize) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_PixelShader> CreatePixelShader (const void* compiledCode, uint32 compiledCodeByteSize) = 0;

		ENGINE_RI_API virtual void ActivateVertexShader (const RI_VertexShader* vertexShader) = 0;
		ENGINE_RI_API virtual void ActivateHullShader (const RI_HullShader* hullShader) = 0;
		ENGINE_RI_API virtual void ActivateDomainShader (const RI_DomainShader* domainShader) = 0;
		ENGINE_RI_API virtual void ActivateGeometryShader (const RI_GeometryShader* geometryShader) = 0;
		ENGINE_RI_API virtual void ActivatePixelShader (const RI_PixelShader* pixelShader) = 0;

		ENGINE_RI_API virtual void BindVertexShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) = 0;
		ENGINE_RI_API virtual void BindHullShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) = 0;
		ENGINE_RI_API virtual void BindDomainShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) = 0;
		ENGINE_RI_API virtual void BindGeometryShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) = 0;
		ENGINE_RI_API virtual void BindPixelShaderConstantBuffer (const RI_ShaderConstantBuffer* constantBuffer, uint32 index) = 0;

		ENGINE_RI_API virtual void BindVertexShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) = 0;
		ENGINE_RI_API virtual void BindHullShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) = 0;
		ENGINE_RI_API virtual void BindDomainShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) = 0;
		ENGINE_RI_API virtual void BindGeometryShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) = 0;
		ENGINE_RI_API virtual void BindPixelShaderResource (const RI_ShaderResourceView* shaderResourceView, uint32 index) = 0;

		ENGINE_RI_API virtual void BindVertexShaderSampler (const RI_Sampler* sampler, uint32 index) = 0;
		ENGINE_RI_API virtual void BindHullShaderSampler (const RI_Sampler* sampler, uint32 index) = 0;
		ENGINE_RI_API virtual void BindDomainShaderSampler (const RI_Sampler* sampler, uint32 index) = 0;
		ENGINE_RI_API virtual void BindGeometryShaderSampler (const RI_Sampler* sampler, uint32 index) = 0;
		virtual void BindPixelShaderSampler (const RI_Sampler* sampler, uint32 index) = 0;

		ENGINE_RI_API virtual bool CompileShader (const std::wstring& sourcePath, const std::string& entryPoint, EShaderStage shaderStage,
			std::unique_ptr<uint8[]>& compiledCode, uint32& compiledCodeSize) = 0;
		ENGINE_RI_API virtual bool ReflectShaderInformation (const void* compiledCode, uint32 compiledCodeByteSize, RI_Shader* shader) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_RenderTargetView> CreateRenderTargetView (const RI_Texture2D* renderBuffer, uint32 mipmapIndex, uint32 arraySize, uint32 firstArrayIndex) = 0;
		ENGINE_RI_API virtual RenderingResourcePtr<RI_DepthStencilView> CreateDepthStencilView (const RI_Texture2D* depthStencilBuffer, uint32 mipmapIndex, uint32 arraySize, uint32 firstArrayIndex) = 0;
		ENGINE_RI_API virtual void BindRenderTargetViewAndDepthStencilView (const RI_RenderTargetView* renderTargetView, const RI_DepthStencilView* depthStencilView) = 0;
		ENGINE_RI_API virtual void ClearRenderTarget (const RI_RenderTargetView* renderTargetView, float red, float green, float blue, float alpha) = 0;
		ENGINE_RI_API virtual void ClearDepthStencil (const RI_DepthStencilView* depthStencilView, float depth, uint8 stencil) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_RasterizerState> CreateRasterizerState (EFillMode fillMode, EFaceCulling faceCulling, EWindingOrder windingOrder) = 0;
		ENGINE_RI_API virtual void SetRasterizerState (const RI_RasterizerState* rasterizerState) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_DepthStencilState> CreateDepthStencilState (EComparisonFunction depthFunction, EComparisonFunction stencilFunction) = 0;
		ENGINE_RI_API virtual void SetDepthStencilState (const RI_DepthStencilState* depthStencilState) = 0;

		ENGINE_RI_API virtual RenderingResourcePtr<RI_InputLayout> CreateInputLayout (const void* compiledCode, uint32 compiledCodeByteSize) = 0;
		ENGINE_RI_API virtual void SetInputLayout (const RI_InputLayout* inputLayout) = 0;

		ENGINE_RI_API virtual void* GetNativeDevice () = 0;
		ENGINE_RI_API virtual void* GetNativeContext () = 0;

	protected:
		template<class T>
		RenderingResourcePtr<T> CreateRenderingResource ()
		{
			auto newResource = std::make_shared<T> ();

			for (auto& resourceRef : m_resourceRefList)
			{
				if (resourceRef->m_bMarkForDelete && resourceRef.use_count () == 1)
				{
					resourceRef = newResource;

					return newResource;
				}
			}

			//m_resourceRefList.push_back (newResource);

			return newResource;
		}

		void DeleteRenderingResource (const RenderingResourcePtr<RI_Resource>& resource)
		{
			resource->m_bMarkForDelete = true;
		}

	private:
		static PathString ModulePath;
		static RenderingInterface* ModuleInstance;

		std::vector<RenderingResourcePtr<RI_Resource>> m_resourceRefList;
	};
}
