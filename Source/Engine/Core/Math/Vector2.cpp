#include "Engine/Core/Math/Vector2.h"
#include "Engine/Core/Math/Math.h"

namespace GameEngine
{
	const Vector2 Vector2::Zero = Vector2 (0.0f, 0.0f);
	const Vector2 Vector2::One = Vector2 (1.0f, 1.0f);

	Vector2::Vector2 (float x, float y) :
		m_x (x), m_y (y)
	{
	}

	Vector2::Vector2 (const Vector2& other) :
		m_x (other.m_x), m_y (other.m_y)
	{
	}

	Vector2::~Vector2 ()
	{
	}

	bool Vector2::Normalize ()
	{
		const float squaredMagnitude = SqrMagnitude ();

		if (squaredMagnitude > Math::Epsilon)
		{
			const float scale = 1.0f / Math::Sqrt (squaredMagnitude);

			m_x *= scale;
			m_y *= scale;

			return false;
		}

		return true;
	}

	float Vector2::Magnitude () const
	{
		return Math::Sqrt (m_x * m_x + m_y * m_y);
	}

	float Vector2::SqrMagnitude () const
	{
		return m_x * m_x + m_y * m_y;
	}

	Vector2 Vector2::Normalized () const
	{
		const float squaredMagnitude = SqrMagnitude ();

		if (squaredMagnitude > Math::Epsilon)
		{
			const float scale = 1.0f / Math::Sqrt (squaredMagnitude);

			return Vector2 (m_x * scale, m_y * scale);
		}

		return Zero;
	}

	float Vector2::Dot (const Vector2& v1, const Vector2& v2)
	{
		return (v1.m_x * v2.m_x) + (v1.m_y * v2.m_y);
	}

	float Vector2::Distance (const Vector2& v1, const Vector2& v2)
	{
		return (v1 - v2).Magnitude ();
	}

	Vector2 & Vector2::operator= (const Vector2& other)
	{
		m_x = other.m_x;
		m_y = other.m_y;

		return *this;
	}

	bool Vector2::operator== (const Vector2& other) const
	{
		return (m_x == other.m_x) && (m_y == other.m_y);
	}

	bool Vector2::operator!= (const Vector2& other) const
	{
		return (m_x != other.m_x) || (m_y != other.m_y);
	}

	Vector2 Vector2::operator- () const
	{
		return -1.0f * (*this);
	}

	Vector2 Vector2::operator+ (const Vector2& other) const
	{
		return Vector2 (m_x + other.m_x, m_y + other.m_y);
	}

	Vector2 Vector2::operator- (const Vector2& other) const
	{
		return Vector2 (m_x - other.m_x, m_y - other.m_y);
	}

	Vector2 Vector2::operator* (const float scale) const
	{
		return Vector2 (m_x * scale, m_y * scale);
	}

	Vector2 Vector2::operator/ (const float scale) const
	{
		return Vector2 (m_x / scale, m_y / scale);
	}

	Vector2& Vector2::operator+= (const Vector2& other)
	{
		m_x += other.m_x;
		m_y += other.m_y;

		return *this;
	}

	Vector2& Vector2::operator-= (const Vector2& other)
	{
		m_x += other.m_x;
		m_y += other.m_y;

		return *this;
	}

	Vector2& Vector2::operator*= (const float scale)
	{
		m_x *= scale;
		m_y *= scale;

		return *this;
	}

	Vector2& Vector2::operator/= (const float scale)
	{
		m_x *= scale;
		m_y *= scale;

		return *this;
	}

	Vector2 operator* (const float scale, const Vector2& vector)
	{
		return Vector2 (scale * vector.m_x, scale * vector.m_y);
	}
}