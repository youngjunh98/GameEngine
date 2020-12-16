#ifndef INCLUDE_SHADER_PROGRAM
#define INCLUDE_SHADER_PROGRAM

#include "Engine/Core/CoreForward.h"
#include "Engine/RI/RenderingInterfaceResource.h"
#include "Engine/RI/RenderingInterfaceEnum.h"
#include "Engine/Core/Object.h"

namespace GameEngine
{
	class Shader : public Object
	{
	public:
		Shader () {}
		virtual ~Shader () {}

		bool Initialize (const std::wstring & sourcePath);
		virtual void Destroy () override;

		bool IsShaderStageExist (EShaderStage stage) const;
		bool IsShaderResourceExist (EShaderStage stage, const std::string& name) const;
		bool IsShaderParameterExist (EShaderStage stage, const std::string& name) const;

		const ShaderResourceBindingMap& GetShaderResourceBindingMap (EShaderStage stage) const;
		ShaderResourceBinding GetShaderResourceBinding (EShaderStage stage, const std::string& name) const;

		const ShaderParameterMap& GetShaderParameterMap (EShaderStage stage) const;
		ShaderParameter GetShaderParameter (EShaderStage stage, const std::string& name) const;

		RI_InputLayout* GetInputLayout () const;

		RI_VertexShader* GetVertexShader () const;
		RI_ShaderConstantBuffer* GetVertexShaderParameterBuffer () const;

		RI_PixelShader* GetPixelShader () const;
		RI_ShaderConstantBuffer* GetPixelShaderParameterBuffer () const;

		RI_HullShader* GetHullShader () const;
		RI_ShaderConstantBuffer* GetHullShaderParameterBuffer () const;

		RI_DomainShader* GetDomainShader () const;
		RI_ShaderConstantBuffer* GetDomainShaderParameterBuffer () const;

		RI_GeometryShader* GetGeometryShader () const;
		RI_ShaderConstantBuffer* GetGeometryShaderParameterBuffer () const;

	private:
		bool InitializeShader (const std::wstring& sourcePath, EShaderStage stage, const std::string& entryPoint, std::unique_ptr<uint8[]>& compiledCode, uint32& compiledCodeSize);
		bool CreateGlobalBuffer (EShaderStage stage);

	private:
		RenderingResourcePtr<RI_InputLayout> m_inputLayout;

		RenderingResourcePtr<RI_VertexShader> m_vertexShader;
		RenderingResourcePtr<RI_ShaderConstantBuffer> m_vertexShaderParameterBuffer;

		RenderingResourcePtr<RI_PixelShader> m_pixelShader;
		RenderingResourcePtr<RI_ShaderConstantBuffer> m_pixelShaderParameterBuffer;

		RenderingResourcePtr<RI_HullShader> m_hullShader;
		RenderingResourcePtr<RI_ShaderConstantBuffer> m_hullShaderParameterBuffer;

		RenderingResourcePtr<RI_DomainShader> m_domainShader;
		RenderingResourcePtr<RI_ShaderConstantBuffer> m_domainShaderParameterBuffer;

		RenderingResourcePtr<RI_GeometryShader> m_geometryShader;
		RenderingResourcePtr<RI_ShaderConstantBuffer> m_geometryShaderParameterBuffer;
	};
}

#endif