#ifndef INCLUDE_COMMON
#define INCLUDE_COMMON

#define PI 3.14159265

float Pow2 (float value)
{
    return value * value;
}

float Pow3 (float value)
{
    float power2 = value * value;
    return power2 * value;
}

float Pow4 (float value)
{
    float power2 = value * value;
    return power2 * power2;
}

float Pow5 (float value)
{
    float power2 = value * value;
    return power2 * power2 * value;
}

float3 Gamma2Linear (float3 color, float gamma)
{
    return pow (abs (color), gamma);
}

float3 Linear2Gamma (float3 color, float gamma)
{
    return pow (abs (color), 1 / gamma);
}

// No includes Pow (color, 1 / 2.2)
float3 TonemapReinhard (float3 color)
{
    return color / (color + 1.0);
}

// Only ALU operations approximation
// Includes the Pow (color, 1 / 2.2)
float3 TonemapFilmic (float3 color)
{
    float3 x = max (0.0, color - 0.004);
    return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}

// TonemapUncharted2 (color) / TonemapUncharted2 (Linear White)
// Linear White = 11.2
// No includes Pow (color, 1 / 2.2)
float3 TonemapUncharted2 (float3 color)
{
    // A = 0.22  Shoulder Strength
    // B = 0.30  Linear Strength
    // C = 0.10  Linear Angle
    // D = 0.20  Toe Strength
    // E = 0.01  Toe Numerator
    // F = 0.30  Toe Denominator
    // ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F
    return ((color * (0.22 * color + 0.1 * 0.3) + 0.2 * 0.01) / (color * (0.22 * color + 0.3) + 0.2 * 0.3)) - (0.01 / 0.3);
}

float Luminance(float3 linearColor )
{
	return dot (linearColor, float3 (0.3, 0.59, 0.11));
}

float2 EquirectangularTextureUV (float3 position)
{
    float2 uv = float2 (atan2 (position.x, position.z), asin (position.y));
    uv = uv / float2 (2.0 * PI, -PI);
    return uv+ 0.5;
}

#endif