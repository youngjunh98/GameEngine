#ifndef INCLUDE_PLANE
#define INCLUDE_PLANE

#include "Engine/Core/Math/Vector3.h"

namespace GameEngine
{
	class Plane
	{
	public:
		Plane () :
			m_normal (Vector3::Zero),
			m_distance (0.0f)
		{}
		Plane (Vector3 normal, float distance) :
			m_normal (normal),
			m_distance (distance)
		{}
		Plane (Vector3 v0, Vector3 v1, Vector3 v2) :
			m_normal (Vector3::Cross (v1 - v0, v2 - v0)),
			m_distance (Vector3::Dot (v0, m_normal))
		{}
		Plane (const Plane& other) :
			m_normal (other.m_normal),
			m_distance (other.m_distance)
		{}
		~Plane ()
		{}

		void Flip ();

		float GetDistanceToPoint (Vector3 point) const;
		Vector3 GetClosestPointOnPlane (Vector3 point) const;

		bool IsContain (Vector3 point) const;
		bool IsPositiveSide (Vector3 point) const;

		Plane& operator= (const Plane& other);
		bool operator== (const Plane& other) const;
		bool operator!= (const Plane& other) const;

	private:
		Vector3 m_normal;
		float m_distance;
	};
}

#endif