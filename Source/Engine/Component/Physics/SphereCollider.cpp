#include "SphereCollider.h"

namespace GameEngine
{
	SphereCollider::SphereCollider ()
	{
	}

	SphereCollider::~SphereCollider ()
	{
	}

	void SphereCollider::OnInit ()
	{
		Collider::OnInit ();

		m_radius = 1.0f;

		g_physics.CreateSphereCollider (*this);
	}

	void SphereCollider::SetRadius (float radius)
	{
		if (m_radius == radius || m_physxShape == nullptr)
		{
			return;
		}

		physx::PxSphereGeometry sphereGeometry (radius);
		m_physxShape->setGeometry (sphereGeometry);
	}

	float SphereCollider::GetRadius () const
	{
		return m_radius;
	}

	void SphereCollider::OnRenderEditor (Editor& editor)
	{
		editor.BeginComponent ("Sphere Collider", this, true);

		editor.AddPropertyFloat ("Radius", m_radius);
		m_radius = Math::Max (m_radius, 0.0f);

		editor.AddPropertyVector3 ("Offset", m_offset);

		if (m_physxShape != nullptr)
		{
			physx::PxSphereGeometry sphereGeometry (m_radius);
			m_physxShape->setGeometry (sphereGeometry);

			physx::PxTransform boxTransform;
			boxTransform.p = physx::PxVec3 (m_offset.m_x, m_offset.m_y, m_offset.m_z);
			boxTransform.q = physx::PxQuat (0.0f, 0.0f, 0.0f, 1.0f);
			m_physxShape->setLocalPose (boxTransform);
		}
	}
}