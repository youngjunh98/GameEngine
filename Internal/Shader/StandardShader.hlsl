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
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
    float3 worldNormal : NORMAL;
	float3 worldTangent : TANGENT;
	float3 worldPosition : TEXCOORD1;
	float3 worldViewDirection : TEXCOORD2;
};

float TilingX;
float TilingY;

VertexOut VertexMain(VertexInput input)
{
	VertexOut output;

	float4 worldPosition = mul (float4 (input.position, 1.0), g_LocalToWorld);
	
	output.position = mul(worldPosition, g_CameraView);
	output.position = mul(output.position, g_CameraProjection);
	output.uv = input.uv * float2 (TilingX, TilingY);

    float3x3 worldMatrixNoTranslation = (float3x3) g_LocalToWorld;

    output.worldNormal = normalize (mul (input.normal, worldMatrixNoTranslation));
	output.worldTangent = normalize (mul (input.tangent, worldMatrixNoTranslation));
	output.worldPosition = worldPosition.xyz;
	output.worldViewDirection = normalize (g_CameraWorldPosition.xyz - worldPosition.xyz);

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

Texture2D MetallicTexture;
SamplerState MetallicTextureSampler;

Texture2D RoughnessTexture;
SamplerState RoughnessTextureSampler;

float4 PixelMain(VertexOut input) : SV_TARGET
{
	float3 viewDir = normalize (input.worldViewDirection);
    float3 eyeDir = normalize (-viewDir);

	float roughness = max (Roughness * RoughnessTexture.Sample (RoughnessTextureSampler, input.uv).r, 0.04);
	float metallic = Metallic * MetallicTexture.Sample (MetallicTextureSampler, input.uv).r;

	float3 baseColor = (BaseColor * BaseColorTexture.Sample(BaseColorTextureSampler, input.uv)).rgb;
    float3 albedo = baseColor - baseColor * metallic;
	float3 specularColor = lerp (0.08 * Specular.xxx, baseColor, metallic.xxx);

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

	float reflectivity = lerp (0.04, 1.0, metallic);
	color += (albedo * irradiance) + (saturate (1 - roughness) * F_Schlick (specularColor, saturate (1.0 - roughness + reflectivity), NdotV) * reflection);

	return float4 (color, 1.0);
}