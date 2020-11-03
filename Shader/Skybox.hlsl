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
	float3 positionLocal : TEXCOORD0;
};

VertexOut VertexMain(VertexInput input)
{
	VertexOut output;

	output.position = float4 (mul(input.position, (float3x3) g_CameraView), 1.0);
	output.position = mul(output.position, g_CameraProjection).xyww;
	
	output.positionLocal = input.position;
	
	return output;
}

#define EXPOSURE 1.0
#define EXPOSURE_BIAS 2.0
#define LINEAR_WHITE 11.2

TextureCube Skybox : register (t0);
SamplerState SkyboxSampler : register (s0);

float4 PixelMain(VertexOut input) : SV_TARGET
{
	float3 color = Gamma2Linear (Skybox.SampleLevel (SkyboxSampler, input.positionLocal, 0).rgb, 2.2);
	float3 tonemappedColor = TonemapUncharted2 (color * (EXPOSURE + EXPOSURE_BIAS)) / TonemapUncharted2 (LINEAR_WHITE);

	return float4 (Linear2Gamma (tonemappedColor, 2.2), 1.0);
}