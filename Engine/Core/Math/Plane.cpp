#include "Core/Math/Plane.h"

namespace GameEngine
{
	void Plane::Flip ()
	{
		m_normal = -m_normal;
	}

	float Plane::GetDistanceToPoint (Vector3 point) const
	{
		return Vector3::Dot (m_normal, point) - m_distance;
	}

	Vector3 Plane::GetClosestPointOnPlane (Vector3 point) const
	{
		return point - GetDistanceToPoint (point) * m_normal;
	}

	bool Plane::IsContain (Vector3 point) const
	{

		return Vector3::Dot (m_normal, point) == m_distance;
	}

	bool Plane::IsPositiveSide (Vector3 point) const
	{
		return Vector3::Dot (m_normal, point) > m_distance;
	}

	Plane& Plane::operator=(const Plane& other)
	{
		m_normal = other.m_normal;
		m_distance = other.m_distance;

		return *this;
	}

	bool Plane::operator==(const Plane& other) const
	{
		return (m_normal == other.m_normal) && (m_distance == other.m_distance);
	}

	bool Plane::operator!=(const Plane& other) const
	{
		return (m_normal != other.m_normal) || (m_distance != other.m_distance);
	}
}