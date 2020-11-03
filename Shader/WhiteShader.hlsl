#include "Common.hlsl"
#include "ShaderVariables.hlsl"

struct VertexInput
{
	float3 position : POSITION;
	float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct VertexOut
{
	float4 position : SV_POSITION;
};

VertexOut VertexMain(VertexInput input)
{
	VertexOut output;

	float4 worldPosition = mul (float4 (input.position, 1.0), g_LocalToWorld);
	
	output.position = mul(worldPosition, g_CameraView);
	output.position = mul(output.position, g_CameraProjection);

	return output;
}

float4 PixelMain(VertexOut input) : SV_TARGET
{
	return float4 (1.0, 1.0, 1.0, 1.0);
}