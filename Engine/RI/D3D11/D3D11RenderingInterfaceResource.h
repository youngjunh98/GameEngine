#ifndef INCLUDE_D3D11_RENDERING_INTERFAEC_RESOURCE
#define INCLUDE_D3D11_RENDERING_INTERFAEC_RESOURCE

#include "RI/D3D11/D3D11.h"
#include "RI/RenderingInterfaceResource.h"

namespace GameEngine
{
	class D3D11RI_VertexBuffer : public RI_VertexBuffer
	{
	public:
		D3D11RI_VertexBuffer ()
		{}
		virtual ~D3D11RI_VertexBuffer ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_resource;
	};

	class D3D11RI_IndexBuffer : public RI_IndexBuffer
	{
	public:
		D3D11RI_IndexBuffer ()
		{}
		virtual ~D3D11RI_IndexBuffer ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_resource;
	};

	class D3D11RI_ShaderConstantBuffer : public RI_ShaderConstantBuffer
	{
	public:
		D3D11RI_ShaderConstantBuffer () {}
		virtual ~D3D11RI_ShaderConstantBuffer ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_resource;
	};

	class D3D11RI_VertexShader : public RI_VertexShader
	{
	public:
		D3D11RI_VertexShader ()
		{}
		virtual ~D3D11RI_VertexShader ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_resource;
	};

	class D3D11RI_HullShader : public RI_HullShader
	{
	public:
		D3D11RI_HullShader ()
		{}
		virtual ~D3D11RI_HullShader ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11HullShader> m_resource;
	};

	class D3D11RI_DomainShader : public RI_DomainShader
	{
	public:
		D3D11RI_DomainShader ()
		{}
		virtual ~D3D11RI_DomainShader ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11DomainShader> m_resource;
	};

	class D3D11RI_GeometryShader : public RI_GeometryShader
	{
	public:
		D3D11RI_GeometryShader ()
		{}
		virtual ~D3D11RI_GeometryShader ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_resource;
	};

	class D3D11RI_PixelShader : public RI_PixelShader
	{
	public:
		D3D11RI_PixelShader ()
		{}
		virtual ~D3D11RI_PixelShader ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_resource;
	};

	class D3D11RI_ShaderResourceView : public RI_ShaderResourceView
	{
	public:
		D3D11RI_ShaderResourceView () {}
		virtual ~D3D11RI_ShaderResourceView ()
		{
			m_resource.Reset ();
		}

		virtual void* GetNative () const override
		{
			return m_resource.Get ();
		}

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resource;
	};

	class D3D11RI_Sampler : public RI_Sampler
	{
	public:
		D3D11RI_Sampler () {}
		virtual ~D3D11RI_Sampler ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_resource;
	};

	class D3D11RI_Texture1D : public RI_Texture1D
	{
	public:
		D3D11RI_Texture1D () {}
		virtual ~D3D11RI_Texture1D ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11Texture1D> m_resource;
	};

	class D3D11RI_Texture2D : public RI_Texture2D
	{
	public:
		D3D11RI_Texture2D () {}
		virtual ~D3D11RI_Texture2D ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_resource;
	};

	class D3D11RI_Texture3D : public RI_Texture3D
	{
	public:
		D3D11RI_Texture3D () {}
		virtual ~D3D11RI_Texture3D ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11Texture3D> m_resource;
	};

	class D3D11RI_RasterizerState : public RI_RasterizerState
	{
	public:
		D3D11RI_RasterizerState () {}
		virtual ~D3D11RI_RasterizerState ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_resource;
	};

	class D3D11RI_DepthStencilState : public RI_DepthStencilState
	{
	public:
		D3D11RI_DepthStencilState () {}
		virtual ~D3D11RI_DepthStencilState ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_resource;
	};

	class D3D11RI_RenderTargetView : public RI_RenderTargetView
	{
	public:
		D3D11RI_RenderTargetView () {}
		virtual ~D3D11RI_RenderTargetView ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_resource;
	};

	class D3D11RI_DepthStencilView : public RI_DepthStencilView
	{
	public:
		D3D11RI_DepthStencilView () {}
		virtual ~D3D11RI_DepthStencilView ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_resource;
	};

	class D3D11RI_InputLayout : public RI_InputLayout
	{
	public:
		D3D11RI_InputLayout () {}
		virtual ~D3D11RI_InputLayout ()
		{
			m_resource.Reset ();
		}

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_resource;
	};
}

#endif