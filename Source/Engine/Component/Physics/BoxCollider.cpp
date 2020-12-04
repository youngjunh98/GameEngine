#include "BoxCollider.h"
#include "Engine/Component/Transform.h"
#include "Editor/EditorGUI.h"

namespace GameEngine
{
	BoxCollider::BoxCollider () : Collider ("Box Collider")
	{
	}

	BoxCollider::~BoxCollider ()
	{
	}

	void BoxCollider::OnInit ()
	{
		Collider::OnInit ();

		m_size = Vector3::One;

		g_physics.CreateBoxCollider (*this);
	}

	void BoxCollider::SetSize (Vector3 size)
	{
		if (m_size == size || m_physxShape == nullptr)
		{
			return;
		}

		m_size = size;

		physx::PxBoxGeometry boxGeometry (0.5f * size.m_x, 0.5f * size.m_y, 0.5f * size.m_z);
		m_physxShape->setGeometry (boxGeometry);
	}

	Vector3 BoxCollider::GetSize () const
	{
		return m_size;
	}

	void BoxCollider::OnRenderEditor ()
	{
		m_size = EditorGUI::InputVector3 ("Size", m_size);
		m_size.m_x = Math::Max (0.0f, m_size.m_x);
		m_size.m_y = Math::Max (0.0f, m_size.m_y);
		m_size.m_z = Math::Max (0.0f, m_size.m_z);

		m_offset = EditorGUI::InputVector3 ("Offset", m_offset);

		if (m_physxShape != nullptr)
		{
			physx::PxBoxGeometry boxGeometry (0.5f * m_size.m_x, 0.5f * m_size.m_y, 0.5f * m_size.m_z);
			m_physxShape->setGeometry (boxGeometry);

			physx::PxTransform boxTransform;
			boxTransform.p = physx::PxVec3 (m_offset.m_x, m_offset.m_y, m_offset.m_z);
			boxTransform.q = physx::PxQuat (0.0f, 0.0f, 0.0f, 1.0f);
			m_physxShape->setLocalPose (boxTransform);
		}
	}
}
