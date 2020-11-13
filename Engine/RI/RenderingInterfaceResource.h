#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "EngineType.h"
#include "RI/RenderingInterfaceEnum.h"

namespace GameEngine
{
	template<class RenderingResourceType>
	using RenderingResourcePtr = std::shared_ptr<RenderingResourceType>;

	class RI_Resource
	{
	public:
		RI_Resource () :
			m_bMarkForDelete (false)
		{}
		virtual ~RI_Resource ()
		{}

		//// For checking validity of resouce.
		//virtual explicit operator bool () const = 0;

		bool m_bMarkForDelete;
	};

	class RI_Buffer : public RI_Resource
	{
	public:
		RI_Buffer () :
			m_bufferByteSize (0),
			m_elementByteSize (0)
		{}
		virtual ~RI_Buffer ()
		{}

		uint32 m_bufferByteSize;
		uint32 m_elementByteSize;
	};

	class RI_VertexBuffer : public RI_Buffer
	{
	public:
		RI_VertexBuffer () :
			m_vertexCount (0)
		{}
		virtual ~RI_VertexBuffer ()
		{}

		uint32 m_vertexCount;
	};

	class RI_IndexBuffer : public RI_Buffer
	{
	public:
		RI_IndexBuffer () :
			m_indexCount (0)
		{}
		virtual ~RI_IndexBuffer ()
		{}

		uint32 m_indexCount;
	};

	class RI_ShaderConstantBuffer : public RI_Buffer
	{
	public:
		RI_ShaderConstantBuffer ()
		{}
		virtual ~RI_ShaderConstantBuffer ()
		{}
	};

	struct ShaderResourceBinding
	{
		enum ResourceType
		{
			None,
			ConstantBuffer,
			Texture,
			Sampler
		};

		ResourceType m_type;
		uint32 m_bindIndex;
		uint32 m_bindCount;
		uint32 m_byteSize;
	};

	struct ShaderParameter
	{
		uint32 m_byteSize;
		uint32 m_byteOffset;
	};

	using ShaderResourceBindingMap = std::unordered_map<std::string, ShaderResourceBinding>;
	using ShaderParameterMap = std::unordered_map<std::string, ShaderParameter>;

	class RI_Shader : public RI_Resource
	{
	public:
		RI_Shader ()
		{}
		virtual ~RI_Shader ()
		{}

		ShaderResourceBindingMap m_bindingMap;
		ShaderParameterMap m_parameterMap;
	};

	class RI_VertexShader : public RI_Shader
	{
	public:
		RI_VertexShader ()
		{}
		virtual ~RI_VertexShader ()
		{}
	};

	class RI_HullShader : public RI_Shader
	{
	public:
		RI_HullShader ()
		{}
		virtual ~RI_HullShader ()
		{}
	};

	class RI_DomainShader : public RI_Shader
	{
	public:
		RI_DomainShader ()
		{}
		virtual ~RI_DomainShader ()
		{}
	};

	class RI_GeometryShader : public RI_Shader
	{
	public:
		RI_GeometryShader ()
		{}
		virtual ~RI_GeometryShader ()
		{}
	};

	class RI_PixelShader : public RI_Shader
	{
	public:
		RI_PixelShader ()
		{}
		virtual ~RI_PixelShader ()
		{}
	};

	class RI_ShaderResourceView : public RI_Resource
	{
	public:
		RI_ShaderResourceView ()
		{}
		virtual ~RI_ShaderResourceView ()
		{}
	};

	class RI_Sampler : public RI_Resource
	{
	public:
		RI_Sampler () : m_addressMode (EAddressMode::Wrap),
			m_filterMode (EFilterMode::Point), m_anisotropicLevel (1)
		{}
		virtual ~RI_Sampler ()
		{}

		EAddressMode m_addressMode;
		EFilterMode m_filterMode;
		uint32 m_anisotropicLevel;
	};

	class RI_Texture : public RI_Resource
	{
	public:
		RI_Texture () :
			m_width (0),
			m_height (0),
			m_mipmapCount (0),
			m_arraySize (0),
			m_format (ERenderingResourceFormat::None)
		{}
		virtual ~RI_Texture ()
		{}

		uint32 m_width;
		uint32 m_height;
		uint32 m_mipmapCount;
		uint32 m_arraySize;
		ERenderingResourceFormat m_format;
	};

	class RI_Texture1D : public RI_Texture
	{
	public:
		RI_Texture1D ()
		{}
		virtual ~RI_Texture1D ()
		{}
	};

	class RI_Texture2D : public RI_Texture
	{
	public:
		RI_Texture2D () : m_bTextureCube (false)
		{}
		virtual ~RI_Texture2D ()
		{}

		bool m_bTextureCube;
	};

	class RI_Texture3D : public RI_Texture
	{
	public:
		RI_Texture3D () :
			m_depth (0)
		{}
		virtual ~RI_Texture3D ()
		{}

		uint32 m_depth;
	};

	class RI_RasterizerState : public RI_Resource
	{
	public:
		RI_RasterizerState () :
			m_fillMode (EFillMode::Solid),
			m_faceCulling (EFaceCulling::None),
			m_windingOrder (EWindingOrder::CW)
		{}
		virtual ~RI_RasterizerState ()
		{}

		EFillMode m_fillMode;
		EFaceCulling m_faceCulling;
		EWindingOrder m_windingOrder;
	};

	class RI_DepthStencilState : public RI_Resource
	{
	public:
		RI_DepthStencilState () :
			m_depthFunction (EComparisonFunction::Less),
			m_stencilFunction (EComparisonFunction::Always)
		{}
		virtual ~RI_DepthStencilState ()
		{}

		EComparisonFunction m_depthFunction;
		EComparisonFunction m_stencilFunction;
	};

	class RI_RenderTargetView : public RI_Resource
	{
	public:
		RI_RenderTargetView ()
		{}
		virtual ~RI_RenderTargetView ()
		{}
	};

	class RI_DepthStencilView : public RI_Resource
	{
	public:
		RI_DepthStencilView ()
		{}
		virtual ~RI_DepthStencilView ()
		{}
	};

	class RI_InputLayout : public RI_Resource
	{
	public:
		RI_InputLayout ()
		{}
		virtual ~RI_InputLayout ()
		{}
	};
}
