#include "RI/D3D11/D3D11RenderingInterfaceEnum.h"

namespace GameEngine
{
	DXGI_FORMAT MapRenderingResourceFormatToDXGIFormat (ERenderingResourceFormat format)
	{
		switch (format)
		{
			case ERenderingResourceFormat::R8_UInt:               return DXGI_FORMAT_R8_UINT;
			case ERenderingResourceFormat::R8G8_UInt:             return DXGI_FORMAT_R8G8_UINT;
			//case ERenderingResourceFormat::R8G8B8_UInt:         return DXGI_FORMAT_R8G8B8_UINT;
			case ERenderingResourceFormat::R8G8B8A8_UInt:         return DXGI_FORMAT_R8G8B8A8_UINT;

			case ERenderingResourceFormat::R8_Int:                return DXGI_FORMAT_R8_SINT;
			case ERenderingResourceFormat::R8G8_Int:              return DXGI_FORMAT_R8G8_SINT;
			//case ERenderingResourceFormat::R8G8B8_Int:          return DXGI_FORMAT_R8G8B8_SINT;
			case ERenderingResourceFormat::R8G8B8A8_Int:          return DXGI_FORMAT_R8G8B8A8_SINT;

			case ERenderingResourceFormat::R8_UNorm:              return DXGI_FORMAT_R8_UNORM;
			case ERenderingResourceFormat::R8G8_UNorm:            return DXGI_FORMAT_R8G8_UNORM;
			//case ERenderingResourceFormat::R8G8B8_UNorm:        return DXGI_FORMAT_R8G8B8_UNORM;
			case ERenderingResourceFormat::R8G8B8A8_UNorm:        return DXGI_FORMAT_R8G8B8A8_UNORM;

			//case ERenderingResourceFormat::R8_UNorm_SRGB:       return DXGI_FORMAT_R8_UNORM_SRGB;
			//case ERenderingResourceFormat::R8G8_UNorm_SRGB:     return DXGI_FORMAT_R8G8_UNORM_SRGB;
			//case ERenderingResourceFormat::R8G8B8_UNorm_SRGB:   return DXGI_FORMAT_R8G8B8_UNORM_SRGB;
			case ERenderingResourceFormat::R8G8B8A8_UNorm_SRGB:   return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

			case ERenderingResourceFormat::R8_Norm:               return DXGI_FORMAT_R8_SNORM;
			case ERenderingResourceFormat::R8G8_Norm:             return DXGI_FORMAT_R8G8_SNORM;
			//case ERenderingResourceFormat::R8G8B8_Norm:         return DXGI_FORMAT_R8G8B8_SNORM;
			case ERenderingResourceFormat::R8G8B8A8_Norm:         return DXGI_FORMAT_R8G8B8A8_SNORM;

			case ERenderingResourceFormat::R16_UInt:              return DXGI_FORMAT_R16_UINT;
			case ERenderingResourceFormat::R16G16_UInt:           return DXGI_FORMAT_R16G16_UINT;
			//case ERenderingResourceFormat::R16G16B16_UInt:      return DXGI_FORMAT_R16G16B16_UINT;
			case ERenderingResourceFormat::R16G16B16A16_UInt:     return DXGI_FORMAT_R16G16B16A16_UINT;

			case ERenderingResourceFormat::R16_Int:               return DXGI_FORMAT_R16_SINT;
			case ERenderingResourceFormat::R16G16_Int:            return DXGI_FORMAT_R16G16_SINT;
			//case ERenderingResourceFormat::R16G16B16_Int:       return DXGI_FORMAT_R16G16B16_SINT;
			case ERenderingResourceFormat::R16G16B16A16_Int:      return DXGI_FORMAT_R16G16B16A16_SINT;

			case ERenderingResourceFormat::R16_UNorm:             return DXGI_FORMAT_R16_UNORM;
			case ERenderingResourceFormat::R16G16_UNorm:          return DXGI_FORMAT_R16G16_UNORM;
			//case ERenderingResourceFormat::R16G16B16_UNorm:     return DXGI_FORMAT_R16G16B16_UNORM;
			case ERenderingResourceFormat::R16G16B16A16_UNorm:    return DXGI_FORMAT_R16G16B16A16_UNORM;

			case ERenderingResourceFormat::R16_Norm:              return DXGI_FORMAT_R16_SNORM;
			case ERenderingResourceFormat::R16G16_Norm:           return DXGI_FORMAT_R16G16_SNORM;
			//case ERenderingResourceFormat::R16G16B16_Norm:      return DXGI_FORMAT_R16G16B16_SNORM;
			case ERenderingResourceFormat::R16G16B16A16_Norm:     return DXGI_FORMAT_R16G16B16A16_SNORM;

			case ERenderingResourceFormat::R16_Float:             return DXGI_FORMAT_R16_FLOAT;
			case ERenderingResourceFormat::R16G16_Float:          return DXGI_FORMAT_R16G16_FLOAT;
			//case ERenderingResourceFormat::R16G16B16_Float:     return DXGI_FORMAT_R16G16B16_FLOAT;
			case ERenderingResourceFormat::R16G16B16A16_Float:    return DXGI_FORMAT_R16G16B16A16_FLOAT;

			case ERenderingResourceFormat::R32_UInt:              return DXGI_FORMAT_R32_UINT;
			case ERenderingResourceFormat::R32G32_UInt:           return DXGI_FORMAT_R32G32_UINT;
			case ERenderingResourceFormat::R32G32B32_UInt:        return DXGI_FORMAT_R32G32B32_UINT;
			case ERenderingResourceFormat::R32G32B32A32_UInt:     return DXGI_FORMAT_R32G32B32A32_UINT;

			case ERenderingResourceFormat::R32_Int:               return DXGI_FORMAT_R32_SINT;
			case ERenderingResourceFormat::R32G32_Int:            return DXGI_FORMAT_R32G32_SINT;
			case ERenderingResourceFormat::R32G32B32_Int:         return DXGI_FORMAT_R32G32B32_SINT;
			case ERenderingResourceFormat::R32G32B32A32_Int:      return DXGI_FORMAT_R32G32B32A32_SINT;

			case ERenderingResourceFormat::R32_Float:             return DXGI_FORMAT_R32_FLOAT;
			case ERenderingResourceFormat::R32G32_Float:          return DXGI_FORMAT_R32G32_FLOAT;
			case ERenderingResourceFormat::R32G32B32_Float:       return DXGI_FORMAT_R32G32B32_FLOAT;
			case ERenderingResourceFormat::R32G32B32A32_Float:    return DXGI_FORMAT_R32G32B32A32_FLOAT;

			case ERenderingResourceFormat::D24_UNorm_S8_UInt:     return DXGI_FORMAT_D24_UNORM_S8_UINT;

			case ERenderingResourceFormat::R24G8_Typeless:        return DXGI_FORMAT_R24G8_TYPELESS;
			case ERenderingResourceFormat::R24_UNorm_X8_Typeless: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

			default:                                              return DXGI_FORMAT_UNKNOWN;
		}
	}

	ERenderingResourceFormat MapDXGIFormatToRenderingResourceFormat (DXGI_FORMAT dxgiFormat)
	{
		switch (dxgiFormat)
		{
			case DXGI_FORMAT_R8_UINT:               return ERenderingResourceFormat::R8_UInt;
			case DXGI_FORMAT_R8G8_UINT:             return ERenderingResourceFormat::R8G8_UInt;
			//case DXGI_FORMAT_R8G8B8_UINT:         return ERenderingResourceFormat::R8G8B8_UInt;
			case DXGI_FORMAT_R8G8B8A8_UINT:         return ERenderingResourceFormat::R8G8B8A8_UInt;

			case DXGI_FORMAT_R8_SINT:               return ERenderingResourceFormat::R8_Int;
			case DXGI_FORMAT_R8G8_SINT:             return ERenderingResourceFormat::R8G8_Int;
			//case DXGI_FORMAT_R8G8B8_SINT:         return ERenderingResourceFormat::R8G8B8_Int;
			case DXGI_FORMAT_R8G8B8A8_SINT:         return ERenderingResourceFormat::R8G8B8A8_Int;

			case DXGI_FORMAT_R8_UNORM:              return ERenderingResourceFormat::R8_UNorm;
			case DXGI_FORMAT_R8G8_UNORM:            return ERenderingResourceFormat::R8G8_UNorm;
			//case DXGI_FORMAT_R8G8B8_UNORM:        return ERenderingResourceFormat::R8G8B8_UNorm;
			case DXGI_FORMAT_R8G8B8A8_UNORM:        return ERenderingResourceFormat::R8G8B8A8_UNorm;

			//case DXGI_FORMAT_R8_UNORM_SRGB:       return ERenderingResourceFormat::R8_UNorm_SRGB;
			//case DXGI_FORMAT_R8G8_UNORM_SRGB:     return ERenderingResourceFormat::R8G8_UNorm_SRGB;
			//case DXGI_FORMAT_R8G8B8_UNORM_SRGB:   return ERenderingResourceFormat::R8G8B8_UNorm_SRGB;
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   return ERenderingResourceFormat::R8G8B8A8_UNorm_SRGB;

			case DXGI_FORMAT_R8_SNORM:              return ERenderingResourceFormat::R8_Norm;
			case DXGI_FORMAT_R8G8_SNORM:            return ERenderingResourceFormat::R8G8_Norm;
			//case DXGI_FORMAT_R8G8B8_SNORM:        return ERenderingResourceFormat::R8G8B8_Norm;
			case DXGI_FORMAT_R8G8B8A8_SNORM:        return ERenderingResourceFormat::R8G8B8A8_Norm;

			case DXGI_FORMAT_R16_UINT:              return ERenderingResourceFormat::R16_UInt;
			case DXGI_FORMAT_R16G16_UINT:           return ERenderingResourceFormat::R16G16_UInt;
			//case DXGI_FORMAT_R16G16B16_UINT:      return ERenderingResourceFormat::R16G16B16_UInt;
			case DXGI_FORMAT_R16G16B16A16_UINT:     return ERenderingResourceFormat::R16G16B16A16_UInt;

			case DXGI_FORMAT_R16_SINT:              return ERenderingResourceFormat::R16_Int;
			case DXGI_FORMAT_R16G16_SINT:           return ERenderingResourceFormat::R16G16_Int;
			//case DXGI_FORMAT_R16G16B16_SINT:      return ERenderingResourceFormat::R16G16B16_Int;
			case DXGI_FORMAT_R16G16B16A16_SINT:     return ERenderingResourceFormat::R16G16B16A16_Int;

			case DXGI_FORMAT_R16_UNORM:             return ERenderingResourceFormat::R16_UNorm;
			case DXGI_FORMAT_R16G16_UNORM:          return ERenderingResourceFormat::R16G16_UNorm;
			//case DXGI_FORMAT_R16G16B16_UNORM:     return ERenderingResourceFormat::R16G16B16_UNorm;
			case DXGI_FORMAT_R16G16B16A16_UNORM:    return ERenderingResourceFormat::R16G16B16A16_UNorm;

			case DXGI_FORMAT_R16_SNORM:             return ERenderingResourceFormat::R16_Norm;
			case DXGI_FORMAT_R16G16_SNORM:          return ERenderingResourceFormat::R16G16_Norm;
			//case DXGI_FORMAT_R16G16B16_SNORM:     return ERenderingResourceFormat::R16G16B16_Norm;
			case DXGI_FORMAT_R16G16B16A16_SNORM:    return ERenderingResourceFormat::R16G16B16A16_Norm;

			case DXGI_FORMAT_R16_FLOAT:             return ERenderingResourceFormat::R16_Float;
			case DXGI_FORMAT_R16G16_FLOAT:          return ERenderingResourceFormat::R16G16_Float;
			//case DXGI_FORMAT_R16G16B16_FLOAT:     return ERenderingResourceFormat::R16G16B16_Float;
			case DXGI_FORMAT_R16G16B16A16_FLOAT:    return ERenderingResourceFormat::R16G16B16A16_Float;

			case DXGI_FORMAT_R32_UINT:              return ERenderingResourceFormat::R32_UInt;
			case DXGI_FORMAT_R32G32_UINT:           return ERenderingResourceFormat::R32G32_UInt;
			case DXGI_FORMAT_R32G32B32_UINT:        return ERenderingResourceFormat::R32G32B32_UInt;
			case DXGI_FORMAT_R32G32B32A32_UINT:     return ERenderingResourceFormat::R32G32B32A32_UInt;

			case DXGI_FORMAT_R32_SINT:              return ERenderingResourceFormat::R32_Int;
			case DXGI_FORMAT_R32G32_SINT:           return ERenderingResourceFormat::R32G32_Int;
			case DXGI_FORMAT_R32G32B32_SINT:        return ERenderingResourceFormat::R32G32B32_Int;
			case DXGI_FORMAT_R32G32B32A32_SINT:     return ERenderingResourceFormat::R32G32B32A32_Int;

			case DXGI_FORMAT_R32_FLOAT:             return ERenderingResourceFormat::R32_Float;
			case DXGI_FORMAT_R32G32_FLOAT:          return ERenderingResourceFormat::R32G32_Float;
			case DXGI_FORMAT_R32G32B32_FLOAT:       return ERenderingResourceFormat::R32G32B32_Float;
			case DXGI_FORMAT_R32G32B32A32_FLOAT:    return ERenderingResourceFormat::R32G32B32A32_Float;

			case DXGI_FORMAT_D24_UNORM_S8_UINT:     return ERenderingResourceFormat::D24_UNorm_S8_UInt;

			case DXGI_FORMAT_R24G8_TYPELESS:        return ERenderingResourceFormat::R24G8_Typeless;
			case DXGI_FORMAT_R24_UNORM_X8_TYPELESS: return ERenderingResourceFormat::R24_UNorm_X8_Typeless;

			default:                                return ERenderingResourceFormat::None;
		}
	}

	D3D11_PRIMITIVE_TOPOLOGY MapPrimitiveTopologyToD3D11PrimitiveTopology (EPrimitiveTopology topology)
	{
		switch (topology)
		{
			case EPrimitiveTopology::PointList:         return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			case EPrimitiveTopology::LineList:          return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			case EPrimitiveTopology::LineStrip:         return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			case EPrimitiveTopology::TriangleList:      return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			case EPrimitiveTopology::TriangleStrip:     return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			case EPrimitiveTopology::TrianglePatchList: return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
			default:                                    return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}
	}

	EPrimitiveTopology MapD3D11PrimitiveTopologyToPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY d3d11PrimitiveTopology)
	{
		switch (d3d11PrimitiveTopology)
		{
			case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST:                 return EPrimitiveTopology::PointList;
			case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:                  return EPrimitiveTopology::LineList;
			case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:                 return EPrimitiveTopology::LineStrip;
			case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:              return EPrimitiveTopology::TriangleList;
			case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:             return EPrimitiveTopology::TriangleStrip;
			case D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST: return EPrimitiveTopology::TrianglePatchList;
			default:                                                 return EPrimitiveTopology::None;
		}
	}

	D3D11_FILL_MODE MapFillModeToD3D11FillMode (EFillMode fillMode)
	{
		switch (fillMode)
		{
			case EFillMode::Wireframe: return D3D11_FILL_WIREFRAME;
			default:                   return D3D11_FILL_SOLID;
		}
	}

	EFillMode MapD3D11FillModeToFillMode (D3D11_FILL_MODE d3d11FillMode)
	{
		switch (d3d11FillMode)
		{
			case D3D11_FILL_WIREFRAME: return EFillMode::Wireframe;
			default:                   return EFillMode::Solid;
		}
	}

	D3D11_CULL_MODE MapFaceCullingToD3D11CullMode (EFaceCulling faceCulling)
	{
		switch (faceCulling)
		{
			case EFaceCulling::Front: return D3D11_CULL_FRONT;
			case EFaceCulling::Back:  return D3D11_CULL_BACK;
			default:                  return D3D11_CULL_NONE;
		}
	}

	EFaceCulling MapD3D11CullModeToFaceCulling (D3D11_CULL_MODE d33d11CullMode)
	{
		switch (d33d11CullMode)
		{
			case D3D11_CULL_FRONT: return EFaceCulling::Front;
			case D3D11_CULL_BACK:  return EFaceCulling::Back;
			default:               return EFaceCulling::None;
		}
	}

	D3D11_COMPARISON_FUNC MapComparisonFunctionToD3D11ComparisonFunc (EComparisonFunction comparisonFunction)
	{
		switch (comparisonFunction)
		{
			case EComparisonFunction::Never:        return D3D11_COMPARISON_NEVER;
			case EComparisonFunction::Less:         return D3D11_COMPARISON_LESS;
			case EComparisonFunction::LessEqual:    return D3D11_COMPARISON_LESS_EQUAL;
			case EComparisonFunction::Equal:        return D3D11_COMPARISON_EQUAL;
			case EComparisonFunction::NotEqual:     return D3D11_COMPARISON_NOT_EQUAL;
			case EComparisonFunction::GreaterEqual: return D3D11_COMPARISON_GREATER_EQUAL;
			case EComparisonFunction::Greaater:     return D3D11_COMPARISON_GREATER;
			default:                                return D3D11_COMPARISON_ALWAYS;
		}
	}

	EComparisonFunction MapD3D11ComparisonFuncToComparisonFunction (D3D11_COMPARISON_FUNC d3d11ComparisonFunc)
	{
		switch (d3d11ComparisonFunc)
		{
			case D3D11_COMPARISON_NEVER:         return EComparisonFunction::Never;
			case D3D11_COMPARISON_LESS:          return EComparisonFunction::Less;
			case D3D11_COMPARISON_LESS_EQUAL:    return EComparisonFunction::LessEqual;
			case D3D11_COMPARISON_EQUAL:         return EComparisonFunction::Equal;
			case D3D11_COMPARISON_NOT_EQUAL:     return EComparisonFunction::NotEqual;
			case D3D11_COMPARISON_GREATER_EQUAL: return EComparisonFunction::GreaterEqual;
			case D3D11_COMPARISON_GREATER:       return EComparisonFunction::Greaater;
			default:                             return EComparisonFunction::Always;
		}
	}

	D3D11_FILTER MapFilterModeToD3D11Filter (EFilterMode filterMode)
	{
		switch (filterMode)
		{
			case EFilterMode::Linear:      return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			case EFilterMode::Bilinear:    return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			case EFilterMode::Trilinear:   return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			case EFilterMode::Anisotropic: return D3D11_FILTER_ANISOTROPIC;
			default:                       return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		}
	}

	EFilterMode MapD3D11FilterToFilterMode (D3D11_FILTER d3d11Filter)
	{
		switch (d3d11Filter)
		{
			case D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT: return EFilterMode::Linear;
			case D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT: return EFilterMode::Bilinear;
			case D3D11_FILTER_MIN_MAG_MIP_LINEAR:       return EFilterMode::Trilinear;
			case D3D11_FILTER_ANISOTROPIC:              return EFilterMode::Anisotropic;
			default:                                    return EFilterMode::Point;
		}
	}

	D3D11_TEXTURE_ADDRESS_MODE MapAddressModeToD3D11TextureAddressMode (EAddressMode addressMode)
	{
		switch (addressMode)
		{
			case EAddressMode::Wrap:   return D3D11_TEXTURE_ADDRESS_WRAP;
			case EAddressMode::Border: return D3D11_TEXTURE_ADDRESS_BORDER;
			default:                   return D3D11_TEXTURE_ADDRESS_CLAMP;
		}
	}
	EAddressMode MapD3D11TextureAddressModeToAddressMode (D3D11_TEXTURE_ADDRESS_MODE d3d11TextureAddressMode)
	{
		switch (d3d11TextureAddressMode)
		{
			case D3D11_TEXTURE_ADDRESS_WRAP:   return EAddressMode::Wrap;
			case D3D11_TEXTURE_ADDRESS_BORDER: return EAddressMode::Border;
			default:                           return EAddressMode::Clamp;
		}
	}
}