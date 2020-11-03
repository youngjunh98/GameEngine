#ifndef INCLUDE_RENDERING_INTERFACE_ENUM
#define INCLUDE_RENDERING_INTERFACE_ENUM

namespace GameEngine
{
	enum class EShaderStage
	{
		// DirectX -> Vertex Shader
		// OpenGL -> Vertex Shader
		Vertex, 

		// DirectX -> Hull Shader
		// OpenGL -> Tessellation Control Shader
		Hull, 

		// DirectX -> Domain Shader
		// OpenGL -> Tessellation Evaluation Shader
		Domain,

		// DirectX -> Geometry Shader
		// OpenGL -> Geometry Shader
		Geometry,

		// DirectX -> Pixel Shader
		// OpenGL -> Fragment Shader
		Pixel
	};

	enum class ERenderingResourceFormat
	{
		None,


		/* 8-Bit Unsigned Int */

		R8_UInt,
		R8G8_UInt,
		R8G8B8_UInt,
		R8G8B8A8_UInt,


		/* 8-Bit Signed Int */

		R8_Int,
		R8G8_Int,
		R8G8B8_Int,
		R8G8B8A8_Int,


		/* 8-Bit Unsigned Normalized */

		R8_UNorm,
		R8G8_UNorm,
		R8G8B8_UNorm,
		R8G8B8A8_UNorm,


		/* 8-Bit Unsigned Normalized SRGB */

		R8_UNorm_SRGB,
		R8G8_UNorm_SRGB,
		R8G8B8_UNorm_SRGB,
		R8G8B8A8_UNorm_SRGB,


		/* 8-Bit Signed Normalized */

		R8_Norm,
		R8G8_Norm,
		R8G8B8_Norm,
		R8G8B8A8_Norm,


		/* 16-Bit Unsigned Int*/ 

		R16_UInt,
		R16G16_UInt,
		R16G16B16_UInt,
		R16G16B16A16_UInt,


		/* 16-Bit Signed Int */

		R16_Int,
		R16G16_Int,
		R16G16B16_Int,
		R16G16B16A16_Int,


		/* 16-Bit Unsigned Normalized */

		R16_UNorm,
		R16G16_UNorm,
		R16G16B16_UNorm,
		R16G16B16A16_UNorm,


		/* 16-Bit Signed Normalized */

		R16_Norm,
		R16G16_Norm,
		R16G16B16_Norm,
		R16G16B16A16_Norm,


		/* 16-Bit Signed Float */

		R16_Float,
		R16G16_Float,
		R16G16B16_Float,
		R16G16B16A16_Float,


		/* 32-Bit Unsigned Int */

		R32_UInt,
		R32G32_UInt,
		R32G32B32_UInt,
		R32G32B32A32_UInt,


		/* 32-Bit Signed Int */

		R32_Int,
		R32G32_Int,
		R32G32B32_Int,
		R32G32B32A32_Int,


		/* 32-Bit Signed Float */

		R32_Float,
		R32G32_Float,
		R32G32B32_Float,
		R32G32B32A32_Float,


		/* Depth Stencil */

		D24_UNorm_S8_UInt,

		/* For Shaow Map */
		R24G8_Typeless,
		R24_UNorm_X8_Typeless
	};

	enum class EPrimitiveTopology
	{
		None,


		/* Point */

		PointList,


		/* Line */

		LineList,
		LineStrip,


		/* Triangle */

		TriangleList,
		TriangleStrip,


		/* Tessellation */

		TrianglePatchList
	};

	enum class EFillMode
	{
		Solid,
		Wireframe
	};

	enum class EFaceCulling
	{
		None,
		Front,
		Back
	};

	enum class EWindingOrder
	{
		/* Clockwise*/

		CW,


		/* Counterclockwise */

		CCW
	};

	enum class EComparisonFunction
	{
		Never,
		Less,
		LessEqual,
		Equal,
		NotEqual,
		GreaterEqual,
		Greaater,
		Always
	};

	enum class EFilterMode
	{
		Point,
		Linear,
		Bilinear,
		Trilinear,
		Anisotropic,
	};

	enum class EAddressMode
	{
		Wrap,
		Clamp,
		Border
	};
}

#endif