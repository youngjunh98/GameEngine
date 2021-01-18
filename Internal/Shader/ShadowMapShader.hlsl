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
    
    output.position = mul(float4 (input.position, 1.0), g_LocalToWorld);
	output.position = mul(output.position, g_CameraViewProjection);

	return output;
}