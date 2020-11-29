#ifndef INCLUDE_MATH
#define INCLUDE_MATH

#include "Type.h"
#include "Core/CoreMacro.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix4x4.h"
#include "Core/Math/AABB.h"
#include "Core/Math/Plane.h"

namespace GameEngine
{
	class CORE_API Math
	{
	public:
		Math () {}
		virtual ~Math () = 0 {}

		static float  Sqrt (float value);
		static double Sqrt (double value);

		static float  Pow (float value, float power);
		static double Pow (double value, double power);

		static float  Exp (float value);
		static double Exp (double value);

		static float  Log (float value, float base);
		static double Log (double value, double base);

		static float  LogNatural (float value);
		static double LogNatural (double value);

		static float  Log10 (float value);
		static double Log10 (double value);


		static float  Sin (float radians);
		static double Sin (double radians);

		static float  Cos (float radians);
		static double Cos (double radians);

		static float  Tan (float radians);
		static double Tan (double radians);

		static float  ArcSin (float value);
		static double ArcSin (double value);

		static float  ArcCos (float value);
		static double ArcCos (double value);

		static float  ArcTan (float value);
		static double ArcTan (double value);

		static float  ArcTan2 (float y, float x);
		static double ArcTan2 (double y, double x);


		static float  Absolute (float value);
		static double Absolute (double value);
		static int32  Absolute (int32 value);
		static int64  Absolute (int64 value);

		static float  Sign (float value);
		static double Sign (double value);

		static float  Ceil (float value);
		static double Ceil (double value);
		static int32  CeilToInt (float value);
		static int64  CeilToInt64 (float value);

		static float  Floor (float value);
		static double Floor (double value);
		static int32  FloorToInt (float value);
		static int64  FloorToInt64 (float value);

		static float  Round (float value);
		static double Round (double value);
		static int32  RoundToInt (float value);
		static int64  RoundToInt64 (float value);

		static float  Remainder (float value, float denominator);
		static double Remainder (double value, double denominator);


		static float  Min (float a, float b);
		static double Min (double a, double b);
		static int32  Min (int32 a, int32 b);
		static int64  Min (int64 a, int64 b);

		static float  Max (float a, float b);
		static double Max (double a, double b);
		static int32  Max (int32 a, int32 b);
		static int64  Max (int64 a, int64 b);

		static float  Clamp (float value, float min, float max);
		static double Clamp (double value, double min, double max);
		static int32  Clamp (int32 value, int32 min, int32 max);
		static int64  Clamp (int64 value, int64 min, int64 max);

	public:
		static const float Pi;
		static const float RcpPi;
		static const float Deg2Rad;
		static const float Rad2Deg;
		static const float Epsilon;
		static const float Infinity;
	};
}

#endif