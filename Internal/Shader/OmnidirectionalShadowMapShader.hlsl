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
    float3 worldPosition : TEXCOORD0;
};

VertexOut VertexMain(VertexInput input)
{
    VertexOut output;
    
    output.position = mul(float4 (input.position, 1.0), g_LocalToWorld);
    output.worldPosition = output.position.xyz;

	output.position = mul(output.position, g_CameraViewProjection);

	return output;
}

float4 PixelMain(VertexOut input) : SV_TARGET
{
    float toSurfaceLength = length (input.worldPosition - g_CameraWorldPosition);
    toSurfaceLength /= max (g_CameraFar, 0.0000001);

	return float4 (toSurfaceLength, 0, 0, 0);
}