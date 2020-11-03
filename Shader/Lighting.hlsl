#ifndef INCLUDE_LIGHTING
#define INCLUDE_LIGHTING

#include "Common.hlsl"
#include "ShaderVariables.hlsl"

float3 CalculateLightPowerAndDirection (LightData lightData, out float3 lightDir, float3 worldPosition, float3 worldNormal)
{
    float3 lightPower = 0;
    lightDir = 0;

    if (lightData.Type == LIGHT_DIRECTIONAL)
    {
        lightDir = normalize (-lightData.WorldDirection.xyz);
        lightPower = lightData.Color.rgb * lightData.Intensity;
    }
    else if (lightData.Type == LIGHT_POINT)
    {
        float3 toLight = lightData.WorldPosition.xyz - worldPosition;
        float surfaceDistance = max (length (toLight), 0.00001);

        lightDir = toLight / surfaceDistance;

        if (surfaceDistance <= lightData.Range)
        {
            float NdotL = saturate (dot (worldNormal, lightDir));
            lightPower = lightData.Color.rgb * lightData.Intensity * NdotL / Pow2 (surfaceDistance);
        }
    }
    else if (lightData.Type == LIGHT_SPOT)
    {
        float3 toSurface = worldPosition - lightData.WorldPosition.xyz;
        float surfaceDistance = max (length (toSurface), 0.00001);

        toSurface /= surfaceDistance;
        lightDir = -toSurface;

        if (surfaceDistance <= lightData.Range)
        {
            float cosSurface = max (dot (toSurface, normalize (lightData.WorldDirection.xyz)), 0.0);
            float cosHalfAngle = cos (lightData.SpotAngle * 0.5);

            if (cosSurface >= cosHalfAngle)
            {
                float NdotL = saturate (dot (worldNormal, lightDir));
                float spot = saturate ((cosSurface - cosHalfAngle) / (1.0 - cosHalfAngle));

                lightPower = lightData.Color.rgb * lightData.Intensity * NdotL * spot / Pow2 (surfaceDistance);
            }
        }
    }

    return lightPower;
}

float CalculateShadow (LightData lightData, int lightIndex, float3 worldPosition)
{
    float shadow = 0.0;

    if (lightData.ShadowType != SHADOW_NONE)
    {
        float currentDepth = 0.0;
        float shadowDepth = 0.0;

        if (lightData.Type == LIGHT_DIRECTIONAL)
        {
            float4 shadowCoords = mul (float4 (worldPosition, 1.0), lightData.ViewProjection);
            shadowCoords.y = -shadowCoords.y;
            shadowCoords.xy = 0.5 * shadowCoords.xy + 0.5;

            currentDepth = saturate (shadowCoords.z);
            shadowDepth = g_DirectionalLightShadowMap.SampleLevel (g_ShadowMapSampler, float3 (shadowCoords.xy, lightIndex), 0).r;
        }
        else if (lightData.Type == LIGHT_POINT)
        {
            float3 toSurface = worldPosition - lightData.WorldPosition.xyz;
            float surfaceDistance = length (toSurface);

            if (surfaceDistance <= lightData.Range)
            {
                currentDepth = surfaceDistance / lightData.Range;
                shadowDepth = g_PointLightShadowMap.SampleLevel (g_ShadowMapSampler, float4 (toSurface, lightIndex), 0).r;
            }
        }
        else if (lightData.Type == LIGHT_SPOT)
        {
            float surfaceDistance = distance (lightData.WorldPosition.xyz, worldPosition);

            if (surfaceDistance <= lightData.Range)
            {
                float4 shadowCoords = mul (float4 (worldPosition, 1.0), lightData.ViewProjection);
                shadowCoords.xyz /= shadowCoords.w;
                shadowCoords.y = -shadowCoords.y;
                shadowCoords.xy = 0.5 * shadowCoords.xy + 0.5;

                currentDepth = saturate (shadowCoords.z);
                shadowDepth = g_SpotLightShadowMap.SampleLevel (g_ShadowMapSampler, float3 (shadowCoords.xy, lightIndex), 0).r;
            }
        }

        shadowDepth += lightData.ShadowDepthBias;

        if (shadowDepth < currentDepth)
        {
            shadow = saturate (lightData.ShadowIntensity);
        }
    }

    return shadow;
}

#endif