#include <memory>

#include "Shader.h"
#include "Engine/Rendering/GlobalRenderer.h"

namespace GameEngine
{
	bool Shader::Initialize (const std::wstring& sourcePath)
	{
		RenderingInterface& renderingInterface = GlobalRenderer::GetRenderingInterface ();
		std::unique_ptr<uint8[]> compiledCode;
		uint32 compiledCodeSize;

		if (InitializeShader (sourcePath, EShaderStage::Vertex, "VertexMain", compiledCode, compiledCodeSize) == false)
		{
			return false;
		}

		m_inputLayout = renderingInterface.CreateInputLayout (compiledCode.get (), compiledCodeSize);

		if (m_inputLayout == nullptr)
		{
			return false;
		}

		if (InitializeShader (sourcePath, EShaderStage::Pixel, "PixelMain", compiledCode, compiledCodeSize) == false)
		{
			return false;
		}

		if (InitializeShader (sourcePath, EShaderStage::Hull, "HullMain", compiledCode, compiledCodeSize) == false)
		{
			return false;
		}

		if (InitializeShader (sourcePath, EShaderStage::Domain, "DomainMain", compiledCode, compiledCodeSize) == false)
		{
			return false;
		}

		if (InitializeShader (sourcePath, EShaderStage::Geometry, "GeometryMain", compiledCode, compiledCodeSize) == false)
		{
			return false;
		}

		return true;
	}

	void Shader::Destroy ()
	{
		Object::Destroy ();

		m_inputLayout = nullptr;

		m_vertexShader = nullptr;
		m_vertexShaderParameterBuffer = nullptr;

		m_pixelShader = nullptr;
		m_pixelShaderParameterBuffer = nullptr;

		m_hullShader = nullptr;
		m_hullShaderParameterBuffer = nullptr;

		m_domainShader = nullptr;
		m_domainShaderParameterBuffer = nullptr;

		m_geometryShader = nullptr;
		m_geometryShaderParameterBuffer = nullptr;
	}

	bool Shader::IsShaderStageExist (EShaderStage stage) const
	{
		if (stage == EShaderStage::Vertex && m_vertexShader != nullptr)
		{
			return true;
		}
		else if (stage == EShaderStage::Pixel && m_pixelShader != nullptr)
		{
			return true;
		}
		else if (stage == EShaderStage::Hull && m_hullShader != nullptr)
		{
			return true;
		}
		else if (stage == EShaderStage::Domain && m_domainShader != nullptr)
		{
			return true;
		}
		else if (stage == EShaderStage::Geometry && m_geometryShader != nullptr)
		{
			return true;
		}

		return false;
	}

	bool Shader::IsShaderResourceExist (EShaderStage stage, const std::string& name) const
	{
		if (stage == EShaderStage::Vertex && m_vertexShader != nullptr)
		{
			if (m_vertexShader->m_bindingMap.find (name) != m_vertexShader->m_bindingMap.end ())
			{
				return true;
			}
		}
		else if (stage == EShaderStage::Pixel && m_pixelShader != nullptr)
		{
			if (m_pixelShader->m_bindingMap.find (name) != m_pixelShader->m_bindingMap.end ())
			{
				return true;
			}
		}
		else if (stage == EShaderStage::Hull && m_hullShader != nullptr)
		{
			if (m_hullShader->m_bindingMap.find (name) != m_hullShader->m_bindingMap.end ())
			{
				return true;
			}
		}
		else if (stage == EShaderStage::Domain && m_domainShader != nullptr)
		{
			if (m_domainShader->m_bindingMap.find (name) != m_domainShader->m_bindingMap.end ())
			{
				return true;
			}
		}
		else if (stage == EShaderStage::Geometry && m_geometryShader != nullptr)
		{
			if (m_geometryShader->m_bindingMap.find (name) != m_geometryShader->m_bindingMap.end ())
			{
				return true;
			}
		}

		return false;
	}

	bool Shader::IsShaderParameterExist (EShaderStage stage, const std::string& name) const
	{
		if (stage == EShaderStage::Vertex && m_vertexShader != nullptr)
		{
			if (m_vertexShader->m_parameterMap.find (name) != m_vertexShader->m_parameterMap.end ())
			{
				return true;
			}
		}
		else if (stage == EShaderStage::Pixel && m_pixelShader != nullptr)
		{
			if (m_pixelShader->m_parameterMap.find (name) != m_pixelShader->m_parameterMap.end ())
			{
				return true;
			}
		}
		else if (stage == EShaderStage::Hull && m_hullShader != nullptr)
		{
			if (m_hullShader->m_parameterMap.find (name) != m_hullShader->m_parameterMap.end ())
			{
				return true;
			}
		}
		else if (stage == EShaderStage::Domain && m_domainShader != nullptr)
		{
			if (m_domainShader->m_parameterMap.find (name) != m_domainShader->m_parameterMap.end ())
			{
				return true;
			}
		}
		else if (stage == EShaderStage::Geometry && m_geometryShader != nullptr)
		{
			if (m_geometryShader->m_parameterMap.find (name) != m_geometryShader->m_parameterMap.end ())
			{
				return true;
			}
		}

		return false;
	}

	const ShaderResourceBindingMap& Shader::GetShaderResourceBindingMap (EShaderStage stage) const
	{
		if (stage == EShaderStage::Vertex)
		{
			return m_vertexShader->m_bindingMap;
		}
		else if (stage == EShaderStage::Pixel)
		{
			return m_pixelShader->m_bindingMap;
		}
		else if (stage == EShaderStage::Hull)
		{
			return m_hullShader->m_bindingMap;
		}
		else if (stage == EShaderStage::Domain)
		{
			return m_domainShader->m_bindingMap;
		}
		else // if (stage == EShaderStage::Geometry)
		{
			return m_geometryShader->m_bindingMap;
		}
	}

	ShaderResourceBinding Shader::GetShaderResourceBinding (EShaderStage stage, const std::string& name) const
	{
		ShaderResourceBinding resourceBinding = { };

		if (stage == EShaderStage::Vertex)
		{
			if (m_vertexShader->m_bindingMap.find (name) != m_vertexShader->m_bindingMap.end ())
			{
				resourceBinding = m_vertexShader->m_bindingMap.at (name);
			}
		}
		else if (stage == EShaderStage::Pixel)
		{
			if (m_pixelShader->m_bindingMap.find (name) != m_pixelShader->m_bindingMap.end ())
			{
				resourceBinding = m_pixelShader->m_bindingMap.at (name);
			}
		}
		else if (stage == EShaderStage::Hull)
		{
			if (m_hullShader->m_bindingMap.find (name) != m_hullShader->m_bindingMap.end ())
			{
				resourceBinding = m_hullShader->m_bindingMap.at (name);
			}
		}
		else if (stage == EShaderStage::Domain)
		{
			if (m_domainShader->m_bindingMap.find (name) != m_domainShader->m_bindingMap.end ())
			{
				resourceBinding = m_domainShader->m_bindingMap.at (name);
			}
		}
		else if (stage == EShaderStage::Geometry)
		{
			if (m_geometryShader->m_bindingMap.find (name) != m_geometryShader->m_bindingMap.end ())
			{
				resourceBinding = m_geometryShader->m_bindingMap.at (name);
			}
		}

		return resourceBinding;
	}

	const ShaderParameterMap& Shader::GetShaderParameterMap (EShaderStage stage) const
	{
		if (stage == EShaderStage::Vertex)
		{
			return m_vertexShader->m_parameterMap;
		}
		else if (stage == EShaderStage::Pixel)
		{
			return m_pixelShader->m_parameterMap;
		}
		else if (stage == EShaderStage::Hull)
		{
			return m_hullShader->m_parameterMap;
		}
		else if (stage == EShaderStage::Domain)
		{
			return m_domainShader->m_parameterMap;
		}
		else // if (stage == EShaderStage::Geometry)
		{
			return m_geometryShader->m_parameterMap;
		}
	}

	ShaderParameter Shader::GetShaderParameter (EShaderStage stage, const std::string& name) const
	{
		ShaderParameter parameter = { };

		if (stage == EShaderStage::Vertex)
		{
			if (m_vertexShader->m_parameterMap.find (name) != m_vertexShader->m_parameterMap.end ())
			{
				parameter = m_vertexShader->m_parameterMap.at (name);
			}
		}
		else if (stage == EShaderStage::Pixel)
		{
			if (m_pixelShader->m_parameterMap.find (name) != m_pixelShader->m_parameterMap.end ())
			{
				parameter = m_pixelShader->m_parameterMap.at (name);
			}
		}
		else if (stage == EShaderStage::Hull)
		{
			if (m_hullShader->m_parameterMap.find (name) != m_hullShader->m_parameterMap.end ())
			{
				parameter = m_hullShader->m_parameterMap.at (name);
			}
		}
		else if (stage == EShaderStage::Domain)
		{
			if (m_domainShader->m_parameterMap.find (name) != m_domainShader->m_parameterMap.end ())
			{
				parameter = m_domainShader->m_parameterMap.at (name);
			}
		}
		else if (stage == EShaderStage::Geometry)
		{
			if (m_geometryShader->m_parameterMap.find (name) != m_geometryShader->m_parameterMap.end ())
			{
				parameter = m_geometryShader->m_parameterMap.at (name);
			}
		}

		return parameter;
	}

	RI_InputLayout* Shader::GetInputLayout () const
	{
		return m_inputLayout.get ();
	}

	RI_VertexShader* Shader::GetVertexShader () const
	{
		return m_vertexShader.get ();
	}

	RI_ShaderConstantBuffer* Shader::GetVertexShaderParameterBuffer () const
	{
		return m_vertexShaderParameterBuffer.get ();
	}

	RI_PixelShader* Shader::GetPixelShader () const
	{
		return m_pixelShader.get ();
	}

	RI_ShaderConstantBuffer* Shader::GetPixelShaderParameterBuffer () const
	{
		return m_pixelShaderParameterBuffer.get ();
	}

	RI_HullShader* Shader::GetHullShader () const
	{
		return m_hullShader.get ();
	}

	RI_ShaderConstantBuffer* Shader::GetHullShaderParameterBuffer () const
	{
		return m_hullShaderParameterBuffer.get ();
	}

	RI_DomainShader* Shader::GetDomainShader () const
	{
		return m_domainShader.get ();
	}

	RI_ShaderConstantBuffer* Shader::GetDomainShaderParameterBuffer () const
	{
		return m_domainShaderParameterBuffer.get ();
	}

	RI_GeometryShader* Shader::GetGeometryShader () const
	{
		return m_geometryShader.get ();
	}

	RI_ShaderConstantBuffer* Shader::GetGeometryShaderParameterBuffer () const
	{
		return m_geometryShaderParameterBuffer.get ();
	}

	bool Shader::InitializeShader (const std::wstring& sourcePath, EShaderStage stage, const std::string& entryPoint, std::unique_ptr<uint8[]>& compiledCode, uint32& compiledCodeSize)
	{
		bool result = true;
		RenderingInterface& renderingInterface = GlobalRenderer::GetRenderingInterface ();

		if (renderingInterface.CompileShader (sourcePath, entryPoint, stage, compiledCode, compiledCodeSize))
		{
			uint8* compiledCodeStart = compiledCode.get ();
			RI_Shader* shader = nullptr;

			if (stage == EShaderStage::Vertex)
			{
				m_vertexShader = renderingInterface.CreateVertexShader (compiledCodeStart, compiledCodeSize);
				shader = m_vertexShader.get ();
			}
			else if (stage == EShaderStage::Pixel)
			{
				m_pixelShader = renderingInterface.CreatePixelShader (compiledCodeStart, compiledCodeSize);
				shader = m_pixelShader.get ();
			}
			else if (stage == EShaderStage::Hull)
			{
				m_hullShader = renderingInterface.CreateHullShader (compiledCodeStart, compiledCodeSize);
				shader = m_hullShader.get ();
			}
			else if (stage == EShaderStage::Domain)
			{
				m_domainShader = renderingInterface.CreateDomainShader (compiledCodeStart, compiledCodeSize);
				shader = m_domainShader.get ();
			}
			else if (stage == EShaderStage::Geometry)
			{
				m_geometryShader = renderingInterface.CreateGeometryShader (compiledCodeStart, compiledCodeSize);
				shader = m_geometryShader.get ();
			}

			if (shader == nullptr)
			{
				result = false;
			}
			else
			{
				if (renderingInterface.ReflectShaderInformation (compiledCodeStart, compiledCodeSize, shader))
				{
					result = CreateGlobalBuffer (stage);
				}
			}
		}

		return result;
	}

	bool Shader::CreateGlobalBuffer (EShaderStage stage)
	{
		bool result = true;
		const auto* globalBufferName = "$Globals";
		RenderingInterface& renderingInterface = GlobalRenderer::GetRenderingInterface ();

		if (IsShaderResourceExist (stage, globalBufferName))
		{
			uint32 globalBufferSize = GetShaderResourceBinding (stage, globalBufferName).m_byteSize;

			if (stage == EShaderStage::Vertex)
			{
				m_vertexShaderParameterBuffer = renderingInterface.CreateShaderConstantBuffer (globalBufferSize, nullptr);
				result = m_vertexShaderParameterBuffer != nullptr;
			}
			else if (stage == EShaderStage::Pixel)
			{
				m_pixelShaderParameterBuffer = renderingInterface.CreateShaderConstantBuffer (globalBufferSize, nullptr);
				result = m_pixelShaderParameterBuffer != nullptr;
			}
			else if (stage == EShaderStage::Hull)
			{
				m_hullShaderParameterBuffer = renderingInterface.CreateShaderConstantBuffer (globalBufferSize, nullptr);
				result = m_hullShaderParameterBuffer != nullptr;
			}
			else if (stage == EShaderStage::Domain)
			{
				m_domainShaderParameterBuffer = renderingInterface.CreateShaderConstantBuffer (globalBufferSize, nullptr);
				result = m_domainShaderParameterBuffer != nullptr;
			}
			else if (stage == EShaderStage::Geometry)
			{
				m_geometryShaderParameterBuffer = renderingInterface.CreateShaderConstantBuffer (globalBufferSize, nullptr);
				result = m_geometryShaderParameterBuffer != nullptr;
			}
		}

		return result;
	}
}