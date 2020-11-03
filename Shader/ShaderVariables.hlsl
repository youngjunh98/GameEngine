#ifndef INCLUDE_SHADER_VARIABLES
#define INCLUDE_SHADER_VARIABLES

cbuffer CBObject
{
    float4x4 g_LocalToWorld;
    float4x4 g_WorldToLocal;
}

cbuffer CBCamera
{
    float4x4 g_CameraView;
    float4x4 g_CameraProjection;
    float4x4 g_CameraViewProjection;
    float3 g_CameraWorldPosition;
    float g_CameraFar;
}

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

#define SHADOW_NONE 0
#define SHADOW_HARD 1
#define SHADOW_SOFT 2 

struct LightData
{
    float4 Color;
    float4 WorldPosition;
    float4 WorldDirection;
    int Type;
    float Intensity;
    float Range;
    float SpotAngle;
    float4x4 ViewProjection;
    int ShadowType;
    float ShadowIntensity;
    float ShadowDepthBias;
};

#define MAX_LIGHT_COUNT 3

cbuffer CBLighting
{
    int g_LightCount;
    float g_ShadowDistance;
    LightData g_LightData[MAX_LIGHT_COUNT];
}

Texture2DArray g_DirectionalLightShadowMap;
TextureCubeArray g_PointLightShadowMap;
Texture2DArray g_SpotLightShadowMap;
SamplerState g_ShadowMapSampler;

TextureCube g_EnvironmentTexture;
SamplerState g_EnvironmentTextureSampler;

#endif