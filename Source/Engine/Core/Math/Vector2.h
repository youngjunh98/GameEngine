#ifndef INCLUDE_VECTOR2
#define INCLUDE_VECTOR2

#include "Engine/Core/CoreMacro.h"

namespace GameEngine
{
	class ENGINE_CORE_API Vector2
	{
	public:
		Vector2 (float x = 0.0f, float y = 0.0f);
		Vector2 (const Vector2& other);
		~Vector2 ();

		bool Normalize ();

		float Magnitude () const;
		float SqrMagnitude () const;
		Vector2 Normalized () const;

		static float Dot (const Vector2& v1, const Vector2& v2);
		static float Distance (const Vector2 & v1, const Vector2& v2);

		Vector2& operator= (const Vector2& other);
		bool operator== (const Vector2& other) const;
		bool operator!= (const Vector2& other) const;
		Vector2 operator- () const;
		Vector2 operator+ (const Vector2& other) const;
		Vector2 operator- (const Vector2& other) const;
		Vector2 operator* (const float scale) const;
		Vector2 operator/ (const float scale) const;
		Vector2& operator+= (const Vector2& other);
		Vector2& operator-= (const Vector2& other);
		Vector2& operator*= (const float scale);
		Vector2& operator/= (const float scale);

	public:
		static const Vector2 Zero;
		static const Vector2 One;

		float m_x, m_y;
	};

	ENGINE_CORE_API Vector2 operator* (const float scale, const Vector2& vector);
}

#endif