#include "Collider.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/Engine/GameObject.h"
#include "Engine/Engine/Component/Transform.h"

namespace GameEngine
{
	Collider::Collider (const std::string& name) : Component (name),
		m_bTrigger (false), m_offset (Vector3::Zero), m_attachedRigidbody (nullptr),
		m_physxShape (nullptr), m_physxRigidStatic (nullptr)
	{
	}

	Collider::~Collider ()
	{
	}

	void Collider::Destroy ()
	{
		if (m_physxRigidStatic != nullptr)
		{
			g_physics.DeleteRigidStatic (*this);

			m_physxRigidStatic->release ();
			m_physxRigidStatic = nullptr;
		}

		if (m_physxShape != nullptr)
		{
			m_physxShape->release ();
			m_physxShape = nullptr;
		}
	}

	void Collider::OnInit ()
	{
		Component::OnInit ();

		SetTrigger (m_bTrigger);
		SetOffset (m_offset);
	}

	void Collider::OnStart ()
	{
		auto& transform = GetGameObject ().GetTransform ();
		auto position = transform.GetPosition ();
		auto rotation = transform.GetRotation ();

		Rigidbody* rigidbody = GetGameObject ().GetComponent<Rigidbody> ();

		if (rigidbody != nullptr)
		{
			g_physics.AttachCollider (*rigidbody, *this);
		}

		SetTrigger (m_bTrigger);
		SetOffset (m_offset);
	}

	void Collider::OnFixedUpdate ()
	{
		if (m_attachedRigidbody == nullptr)
		{
			auto& transform = GetGameObject ().GetTransform ();
			auto position = transform.GetPosition ();
			auto rotation = transform.GetRotation ();

			if (m_physxRigidStatic == nullptr)
			{
				g_physics.CreateRigidStatic (*this, position, Quaternion::Identity);
			}

			physx::PxTransform physxTransform;

			physxTransform.p.x = position.m_x;
			physxTransform.p.y = position.m_y;
			physxTransform.p.z = position.m_z;

			physxTransform.q.x = rotation.m_x;
			physxTransform.q.y = rotation.m_y;
			physxTransform.q.z = rotation.m_z;
			physxTransform.q.w = rotation.m_w;

			m_physxRigidStatic->setGlobalPose (physxTransform);
		}
	}

	bool Collider::GetTrigger () const
	{
		return m_bTrigger;
	}

	void Collider::SetTrigger (bool bIsTrigger)
	{
		m_bTrigger = bIsTrigger;
		physx::PxShapeFlags flags = GetShape ()->getFlags ();
			
		if (bIsTrigger)
		{
			flags.clear (physx::PxShapeFlag::eSIMULATION_SHAPE);
			flags.set (physx::PxShapeFlag::eTRIGGER_SHAPE);		
		}
		else
		{
			flags.clear (physx::PxShapeFlag::eTRIGGER_SHAPE);
			flags.set (physx::PxShapeFlag::eSIMULATION_SHAPE);
		}

		GetShape ()->setFlags (flags);
	}

	Vector3 Collider::GetOffset () const
	{
		return m_offset;
	}

	void Collider::SetOffset (Vector3 offset)
	{
		m_offset = offset;
		GetShape ()->setLocalPose (physx::PxTransform (offset.m_x, offset.m_y, offset.m_z));
	}

	Rigidbody* Collider::GetAttachedRigidbody () const
	{
		return m_attachedRigidbody;
	}

	void Collider::SetAttachedRigidbody (Rigidbody& rigidbody)
	{
		m_attachedRigidbody = &rigidbody;
	}

	void Collider::OnSerialize (Json::Json& json) const
	{
		Component::OnSerialize (json);

		Json::JsonSerializer::Serialize (json, "trigger", m_bTrigger);
		Json::JsonSerializer::Serialize (json, "offset", m_offset);
	}

	void Collider::OnDeserialize (const Json::Json& json)
	{
		Component::OnDeserialize (json);

		SetTrigger (Json::JsonSerializer::Deserialize<bool> (json, "trigger"));
		SetOffset (Json::JsonSerializer::Deserialize<Vector3> (json, "offset"));
	}
}