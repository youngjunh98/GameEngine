#ifndef D3D11_RENDERING_INTERFACE_ENUM
#define D3D11_RENDERING_INTERFACE_ENUM

#include "Engine/RI/D3D11/D3D11.h"
#include "Engine/RI/RenderingInterfaceEnum.h"

namespace GameEngine
{
	/* Rendering Interface Enum Mapping Functions */

	DXGI_FORMAT MapRenderingResourceFormatToDXGIFormat (ERenderingResourceFormat format);
	ERenderingResourceFormat MapDXGIFormatToRenderingResourceFormat (DXGI_FORMAT dxgiFormat);

	D3D11_PRIMITIVE_TOPOLOGY MapPrimitiveTopologyToD3D11PrimitiveTopology (EPrimitiveTopology topology);
	EPrimitiveTopology MapD3D11PrimitiveTopologyToPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY d3d11PrimitiveTopology);

	D3D11_FILL_MODE MapFillModeToD3D11FillMode (EFillMode fillMode);
	EFillMode MapD3D11FillModeToFillMode (D3D11_FILL_MODE d3d11FillMode);

	D3D11_CULL_MODE MapFaceCullingToD3D11CullMode (EFaceCulling faceCulling);
	EFaceCulling MapD3D11CullModeToFaceCulling (D3D11_CULL_MODE d33d11CullMode);

	D3D11_COMPARISON_FUNC MapComparisonFunctionToD3D11ComparisonFunc (EComparisonFunction comparisonFunction);
	EComparisonFunction MapD3D11ComparisonFuncToComparisonFunction (D3D11_COMPARISON_FUNC d3d11ComparisonFunc);

	D3D11_FILTER MapFilterModeToD3D11Filter (EFilterMode filterMode);
	EFilterMode MapD3D11FilterToFilterMode (D3D11_FILTER d3d11Filter);

	D3D11_TEXTURE_ADDRESS_MODE MapAddressModeToD3D11TextureAddressMode (EAddressMode addressMode);
	EAddressMode MapD3D11TextureAddressModeToAddressMode (D3D11_TEXTURE_ADDRESS_MODE d3d11TextureAddressMode);
}

#endif