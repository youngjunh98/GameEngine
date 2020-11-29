#include <cmath>
#include <limits>

#include "Core/Math/Math.h"

namespace GameEngine
{
	const float Math::Pi = 3.141592653f;
	const float Math::RcpPi = 0.318309886f;
	const float Math::Deg2Rad = Math::Pi / 180.0f;
	const float Math::Rad2Deg = 180.0f / Math::Pi;
	const float Math::Epsilon = std::numeric_limits<float>::epsilon ();
	const float Math::Infinity = std::numeric_limits<float>::infinity ();

	float Math::Sqrt (float value)
	{
		return sqrt (value);
	}

	double Math::Sqrt (double value)
	{
		return sqrt (value);
	}

	float Math::Pow (float value, float power)
	{
		return pow (value, power);
	}

	double Math::Pow (double value, double power)
	{
		return pow (value, power);
	}

	float Math::Exp (float value)
	{
		return exp (value);
	}

	double Math::Exp (double value)
	{
		return exp (value);
	}

	float Math::Log (float value, float base)
	{
		return log (value) / log (base);
	}

	double Math::Log (double value, double base)
	{
		return log (value) / log (base);
	}

	float Math::LogNatural (float value)
	{
		return log (value);
	}

	double Math::LogNatural (double value)
	{
		return log (value);
	}

	float Math::Log10 (float value)
	{
		return log10 (value);
	}

	double Math::Log10 (double value)
	{
		return log10 (value);
	}

	float Math::Sin (float radians)
	{
		return sin (radians);
	}

	double Math::Sin (double radians)
	{
		return sin (radians);
	}

	float Math::Cos (float radians)
	{
		return cos (radians);
	}

	double Math::Cos (double radians)
	{
		return cos (radians);
	}

	float Math::Tan (float radians)
	{
		return tan (radians);
	}

	double Math::Tan (double radians)
	{
		return tan (radians);
	}

	float Math::ArcSin (float value)
	{
		return asin (value);
	}

	double Math::ArcSin (double value)
	{
		return asin (value);
	}

	float Math::ArcCos (float value)
	{
		return acos (value);
	}

	double Math::ArcCos (double value)
	{
		return acos (value);
	}

	float Math::ArcTan (float value)
	{
		return atan (value);
	}

	double Math::ArcTan (double value)
	{
		return atan (value);
	}

	float Math::ArcTan2 (float y, float x)
	{
		return atan2 (y, x);
	}

	double Math::ArcTan2 (double y, double x)
	{
		return atan2 (y, x);
	}

	float Math::Absolute (float value)
	{
		return abs (value);
	}

	double Math::Absolute (double value)
	{
		return abs (value);
	}

	int32 Math::Absolute (int32 value)
	{
		return abs (value);
	}

	int64 Math::Absolute (int64 value)
	{
		return abs (value);
	}

	float Math::Sign (float value)
	{
		if (value > 0.0f)
		{
			return 1.0f;
		}
		else if (value < 0.0f)
		{
			return -1.0f;
		}
		else
		{
			return 0.0f;
		}
	}

	double Math::Sign (double value)
	{
		if (value > 0.0)
		{
			return 1.0;
		}
		else if (value < 0.0)
		{
			return -1.0;
		}
		else
		{
			return 0.0;
		}
	}

	float Math::Ceil (float value)
	{
		return ceil (value);
	}

	double Math::Ceil (double value)
	{
		return ceil (value);
	}

	int32 Math::CeilToInt (float value)
	{
		return static_cast<int32> (ceil (value));
	}

	int64 Math::CeilToInt64 (float value)
	{
		return static_cast<int64> (ceil (value));
	}

	float Math::Floor (float value)
	{
		return floor (value);
	}

	double Math::Floor (double value)
	{
		return floor (value);
	}

	int32 Math::FloorToInt (float value)
	{
		return static_cast<int32> (floor (value));
	}

	int64 Math::FloorToInt64 (float value)
	{
		return static_cast<int64> (floor (value));
	}

	float Math::Round (float value)
	{
		return round (value);
	}

	double Math::Round (double value)
	{
		return round (value);
	}

	int32 Math::RoundToInt (float value)
	{
		return static_cast<int32> (round (value));
	}

	int64 Math::RoundToInt64 (float value)
	{
		return static_cast<int64> (round (value));
	}

	float Math::Remainder (float value, float denominator)
	{
		return fmod (value, denominator);
	}

	double Math::Remainder (double value, double denominator)
	{
		return fmod (value, denominator);
	}

	float Math::Min (float a, float b)
	{
		return a < b ? a : b;
	}

	double Math::Min (double a, double b)
	{
		return a < b ? a : b;
	}

	int32 Math::Min (int32 a, int32 b)
	{
		return a < b ? a : b;
	}

	int64 Math::Min (int64 a, int64 b)
	{
		return a < b ? a : b;
	}

	float Math::Max (float a, float b)
	{
		return a > b ? a : b;
	}

	double Math::Max (double a, double b)
	{
		return a > b ? a : b;
	}

	int32 Math::Max (int32 a, int32 b)
	{
		return a > b ? a : b;
	}

	int64 Math::Max (int64 a, int64 b)
	{
		return a > b ? a : b;
	}

	float Math::Clamp (float value, float min, float max)
	{
		return Min (Max (value, min), max);
	}

	double Math::Clamp (double value, double min, double max)
	{
		return Min (Max (value, min), max);
	}

	int32 Math::Clamp (int32 value, int32 min, int32 max)
	{
		return Min (Max (value, min), max);
	}

	int64 Math::Clamp (int64 value, int64 min, int64 max)
	{
		return Min (Max (value, min), max);
	}
}
