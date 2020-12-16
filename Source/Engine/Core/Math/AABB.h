#ifndef INCLUDE_AABB
#define INCLUDE_AABB

#include "Engine/Core/CoreMacro.h"
#include "Engine/Core/Math/Vector3.h"

namespace GameEngine
{
	class ENGINE_CORE_API AABB
	{
	public:
		AABB () :
			m_center (Vector3::Zero),
			m_extent (Vector3::Zero)
		{}
		AABB (Vector3 center, Vector3 size) :
			m_center (center),
			m_extent (size * 0.5f)
		{}
		AABB (const AABB& other) :
			m_center (other.m_center),
			m_extent (other.m_extent)
		{}
		~AABB ()
		{}

		void Include (Vector3 point);
		void Include (AABB aabb);

		bool IsContain (Vector3 point);
		bool IsIntersect (AABB aabb);

		Vector3 GetCenter () const;
		Vector3 GetExtent () const;
		Vector3 GetSize () const;

		Vector3 GetMinimumPoint () const;
		Vector3 GetMaximumPoint () const;

		AABB& operator= (const AABB& other);
		bool operator== (const AABB& other) const;
		bool operator!= (const AABB& other) const;

	private:
		Vector3 m_center;
		Vector3 m_extent;
	};
}

#endif