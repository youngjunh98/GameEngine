#include "Core/Math/AABB.h"
#include "Core/Math/Math.h"

namespace GameEngine
{
	void AABB::Include (Vector3 point)
	{
		Vector3 min = GetMinimumPoint ();
		Vector3 max = GetMaximumPoint ();

		if (point.m_x < min.m_x)
		{
			min.m_x = point.m_x;
		}
		else if (point.m_x > max.m_x)
		{
			max.m_x = point.m_x;
		}

		if (point.m_y < min.m_y)
		{
			min.m_y = point.m_y;
		}
		else if (point.m_y > max.m_y)
		{
			max.m_y = point.m_y;
		}

		if (point.m_z < min.m_z)
		{
			min.m_z = point.m_z;
		}
		else if (point.m_z > max.m_z)
		{
			max.m_z = point.m_z;
		}

		m_extent = 0.5f * (max - min);
		m_center = min + m_extent;
	}

	void AABB::Include (AABB aabb)
	{
		Vector3 min = GetMinimumPoint ();
		Vector3 otherMin = aabb.GetMinimumPoint ();

		if (min.m_x > otherMin.m_x)
		{
			min.m_x = otherMin.m_x;
		}

		if (min.m_y > otherMin.m_y)
		{
			min.m_y = otherMin.m_y;
		}

		if (min.m_z > otherMin.m_z)
		{
			min.m_z = otherMin.m_z;
		}

		Vector3 max = GetMaximumPoint ();
		Vector3 otherMax = aabb.GetMaximumPoint ();

		if (max.m_x < otherMax.m_x)
		{
			max.m_x = otherMax.m_x;
		}

		if (max.m_y < otherMax.m_y)
		{
			max.m_y = otherMax.m_y;
		}

		if (max.m_z < otherMax.m_z)
		{
			max.m_z = otherMax.m_z;
		}

		m_extent = 0.5f * (max - min);
		m_center = min + m_extent;
	}

	bool AABB::IsContain (Vector3 point)
	{
		return false;
	}

	bool AABB::IsIntersect (AABB aabb)
	{
		bool bIntersectX = Math::Absolute (m_center.m_x - aabb.m_center.m_x) <= (m_extent.m_x + aabb.m_extent.m_x);
		bool bIntersectY = Math::Absolute (m_center.m_y - aabb.m_center.m_y) <= (m_extent.m_y + aabb.m_extent.m_y);
		bool bIntersectZ = Math::Absolute (m_center.m_z - aabb.m_center.m_z) <= (m_extent.m_z + aabb.m_extent.m_z);

		return bIntersectX && bIntersectY && bIntersectZ;
	}

	Vector3 AABB::GetCenter () const
	{
		return m_center;
	}

	Vector3 AABB::GetExtent () const
	{
		return m_extent;
	}

	Vector3 AABB::GetSize () const
	{
		return m_extent * 2.0f;
	}

	Vector3 AABB::GetMinimumPoint () const
	{
		return m_center - m_extent;
	}

	Vector3 AABB::GetMaximumPoint () const
	{
		return m_center + m_extent;
	}

	AABB& AABB::operator= (const AABB& other)
	{
		m_center = other.m_center;
		m_extent = other.m_extent;

		return *this;
	}

	bool AABB::operator== (const AABB& other) const
	{
		return (m_center == other.m_center) && (m_extent == other.m_extent);
	}

	bool AABB::operator!= (const AABB& other) const
	{
		return (m_center != other.m_center) || (m_extent != other.m_extent);
	}
}
