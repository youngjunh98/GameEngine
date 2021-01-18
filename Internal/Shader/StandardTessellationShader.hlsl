#include "Common.hlsl"
#include "ShaderVariables.hlsl"
#include "PBR.hlsl"
#include "Lighting.hlsl"

struct VertexInput
{
	float3 position : POSITION;
	float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct VertexOut
{
	float2 uv : TEXCOORD0;
    float3 worldPosition : POSITION;
    float3 worldNormal : NORMAL;
	float3 worldTangent : TANGENT;
	float3 worldViewDirection : TEXCOORD1;
    float TessFactor : TEXCOORD2;
};

float TessellationRange;
float MinTessellation;
float MaxTessellation;
float TilingX;
float TilingY;

VertexOut VertexMain(VertexInput input)
{
	VertexOut output;
	output.uv = input.uv * float2 (TilingX, TilingY);

    float4 worldPosition = mul (float4 (input.position, 1.0), g_LocalToWorld);

    output.worldPosition = worldPosition.xyz;

    float3 view = g_CameraWorldPosition.xyz - worldPosition.xyz;
    float viewDistance = length (view);

    output.worldViewDirection = view / viewDistance;

    float tessellation = 1.0 - saturate (viewDistance / TessellationRange);

    output.TessFactor = MinTessellation + tessellation * (MaxTessellation - MinTessellation);

    float3x3 worldMatrixNoTranslation = (float3x3) g_LocalToWorld;

    output.worldNormal = normalize (mul (input.normal, worldMatrixNoTranslation));
	output.worldTangent = normalize (mul (input.tangent, worldMatrixNoTranslation));

	return output;
}

struct PatchTess
{
    float EdgeTess[3] : SV_TessFactor;
    float InsideTess : SV_InsideTessFactor;
};

PatchTess HullPatch (InputPatch<VertexOut,3> patch, uint patchID : SV_PrimitiveID)
{
    PatchTess output;

    output.EdgeTess[0] = 0.5 * (patch[1].TessFactor + patch[2].TessFactor);
    output.EdgeTess[1] = 0.5 * (patch[2].TessFactor + patch[0].TessFactor);
    output.EdgeTess[2] = 0.5 * (patch[0].TessFactor + patch[1].TessFactor);

    output.InsideTess =  output.EdgeTess[0];

    return output;
}

struct HullOut
{
    float2 uv : TEXCOORD0;
    float3 worldPosition : POSITION;
    float3 worldNormal : NORMAL;
	float3 worldTangent : TANGENT;
	float3 worldViewDirection : TEXCOORD1;
};

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HullPatch")]
HullOut HullMain (InputPatch<VertexOut,3> patch, uint i : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
    HullOut output;
    
    output.uv = patch[i].uv;
    output.worldPosition = patch[i].worldPosition;
    output.worldNormal = patch[i].worldNormal;
    output.worldTangent = patch[i].worldTangent;
    output.worldViewDirection = patch[i].worldViewDirection;

    return output;
}

struct DomainOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 worldPosition : POSITION;
    float3 worldNormal : NORMAL;
	float3 worldTangent : TANGENT;
	float3 worldViewDirection : TEXCOORD1;
};

float DisplacementScale;
Texture2D DisplacementTexture;
SamplerState DisplacementTextureSampler;

[domain("tri")]
DomainOut DomainMain (PatchTess patchTess, float3 barycentric : SV_DomainLocation, const OutputPatch<HullOut,3> tri)
{
    DomainOut output;

    output.worldPosition = barycentric.x * tri[0].worldPosition + barycentric.y * tri[1].worldPosition + barycentric.z * tri[2].worldPosition;
    output.uv = barycentric.x * tri[0].uv + barycentric.y * tri[1].uv + barycentric.z * tri[2].uv;
    output.worldNormal = normalize (barycentric.x * tri[0].worldNormal + barycentric.y * tri[1].worldNormal + barycentric.z * tri[2].worldNormal);
    output.worldTangent = normalize (barycentric.x * tri[0].worldTangent + barycentric.y * tri[1].worldTangent + barycentric.z * tri[2].worldTangent);
    output.worldViewDirection = normalize (barycentric.x * tri[0].worldViewDirection + barycentric.y * tri[1].worldViewDirection + barycentric.z * tri[2].worldViewDirection);

    float mipInterval = 20.0;
    float viewDistance = length (g_CameraWorldPosition.xyz - output.worldPosition);
    float mipLevel = clamp ((viewDistance - mipInterval) / mipInterval, 0.0, 6.0);
    float displacement = DisplacementTexture.SampleLevel (DisplacementTextureSampler, output.uv, mipLevel).r;

    output.worldPosition += (DisplacementScale * (displacement - 1.0)) * output.worldNormal;

    output.position = mul (float4 (output.worldPosition, 1.0), g_CameraView);
    output.position = mul (output.position, g_CameraProjection);

    return output;
}

float4 BaseColor;
float Roughness;
float Metallic;
float Specular;

Texture2D BaseColorTexture;
SamplerState BaseColorTextureSampler;

Texture2D NormalTexture;
SamplerState NormalTextureSampler;

float4 PixelMain(DomainOut input) : SV_TARGET
{
	float3 viewDir = normalize (input.worldViewDirection);
    float3 eyeDir = normalize (-viewDir);

	float roughness = max (Roughness, 0.04);

	float3 baseColor = (BaseColor * BaseColorTexture.Sample(BaseColorTextureSampler, input.uv)).rgb;
    float3 albedo = baseColor - baseColor * Metallic;
	float3 specularColor = lerp (0.08 * Specular.xxx, baseColor, Metallic.xxx);

    // Normal Mapping
	float3 worldNormal = normalize (input.worldNormal);
	float3 worldTangent = normalize (input.worldTangent);
	float3 worldBinormal = normalize (cross (worldNormal, worldTangent));

	float3x3 TBN = float3x3 (worldTangent, worldBinormal, worldNormal);
	float3 tangentNormal = 2.0 * NormalTexture.Sample(NormalTextureSampler, input.uv).rgb - 1.0;

	float3 normal = mul (tangentNormal, TBN);

    // Lighting
    float3 color = 0;

    for (int i = 0; i < g_LightCount; i++)
	{
		float3 lightDir;
		float3 lightPower = CalculateLightPowerAndDirection (g_LightData[i], /* out */ lightDir, input.worldPosition, normal);
		float3 brdf = PBR_BRDF (albedo, specularColor, roughness, normal, viewDir, lightDir);
		float shadow = CalculateShadow (g_LightData[i], i, input.worldPosition);

		color += (1.0 - shadow) * lightPower * brdf;
    }

	// Environment Lighting
	float NdotV = saturate (dot (normal, viewDir));
	float3 eyeReflected = reflect (eyeDir, normal);

	float3 irradiance = g_EnvironmentTexture.SampleLevel (g_EnvironmentTextureSampler, normal, 10.0).rgb;
	float3 reflection = g_EnvironmentTexture.SampleLevel (g_EnvironmentTextureSampler, eyeReflected, roughness * 10.0).rgb;

	color += (albedo * irradiance) + (saturate (1 - roughness + Metallic) * F_Schlick (specularColor, 1.0, NdotV) * reflection);

	return float4 (color, 1.0);
}