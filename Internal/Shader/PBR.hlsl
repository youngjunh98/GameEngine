#ifndef INCLUDE_PBR
#define INCLUDE_PBR

#include "Common.hlsl"
#include "ShaderVariables.hlsl"

float Diffuse_Lambert (float NdotL)
{
	return NdotL;
}

float Diffuse_OrenNayar (float Roughness, float NdotL, float NdotV, float3 N, float3 L, float3 V)
{
	float gamma = dot (V - N * NdotV, L - N * NdotL);

	float roughness2 = Pow2 (Roughness);
	float A = 1.0 - 0.5 * (roughness2 / (roughness2 + 0.33));
	float B = 0.45 * (roughness2 / (roughness2 + 0.09));

	float viewAngle = acos (NdotV);
	float lightAngle = acos (NdotL);
	float alpha = max (viewAngle, lightAngle);
	float beta = min (viewAngle, lightAngle);
	float C = sin (alpha) * tan (beta);

	return A + B * max (gamma, 0.0) * C;
}

// Implement and test later
//float Diffuse_Disney (float Roughness, float NdotV, float NdotL, float LdotH)
//{
//
//}

float D_GGX (float NdotH, float alpha)
{
	// Unity and "Course notes to moving Frostbite to PBR"
	// Apply division by PI later
	float a2 = alpha * alpha;
	float d = (NdotH * a2 - NdotH) * NdotH + 1.0;
	return a2 / (d * d);
}

float Vis_Smith (float NdotL, float NdotV, float alpha)
{
	// Unreal engine 4
	float a2 = alpha * alpha;
	float smith_v = NdotV + sqrt (NdotV * (NdotV - NdotV * a2) + a2);
	float smith_l = NdotL + sqrt (NdotL * (NdotL - NdotL * a2) + a2);
	return 1.0 / (smith_v * smith_l);
}

float Vis_SmithCorrelated (float NdotL, float NdotV, float alpha)
{
	// "Course notes to moving Frostbite to PBR"
	// Height-Correlated Masking and Shadowing
	// GGX Distribution
	float a2 = alpha * alpha;
	float lambda_v = NdotL * sqrt ((-NdotV * a2 + NdotV) * NdotV + a2);
	float lambda_l = NdotV * sqrt ((-NdotL * a2 + NdotL) * NdotL + a2);
	return 0.5 / (lambda_v + lambda_l);
}

float3 F_Schlick (float3 F0, float F90, float LdotH)
{
	// Unity and "Course notes to moving Frostbite to PBR"
	return F0 + (F90 - F0) * Pow5 (1 - LdotH);
}

float3 PBR_BRDF (float3 albedo, float3 specularColor, float roughness, float3 N, float3 V, float3 L)
{
	float NdotV = abs (dot (N, V)) + 0.00001;
	float NdotL = saturate (dot (N, L));
	
	float3 H = normalize (V + L);
	float NdotH = saturate (dot (N, H));
	float LdotH = saturate (dot (L, H));

	float D = D_GGX (NdotH, roughness);
	float Vis = Vis_SmithCorrelated (NdotL, NdotV, roughness);
	float3 F = F_Schlick (specularColor, 1.0, LdotH);

	float3 specular = D * F * Vis;
	float3 diffuse = albedo * Diffuse_Lambert (NdotL);

	return (diffuse + specular * NdotL) / PI;
}

#endif