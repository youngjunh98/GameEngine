#include "Rigidbody.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/Engine/GameObject.h"
#include "Engine/Engine/Component/Transform.h"
#include "Editor/Core/EditorGUI.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (Rigidbody)

	Rigidbody::Rigidbody () : Component ("Rigidbody"),
		m_mass (1.0f),
		m_linearDamping (0.0f),
		m_angularDamping (0.0f),
		m_bGravity (true),
		m_bKinematic (false),
		m_bConstraintPositionX (false),
		m_bConstraintPositionY (false),
		m_bConstraintPositionZ (false),
		m_bConstraintPitch (false),
		m_bConstraintYaw (false),
		m_bConstraintRoll (false),
		m_physxRigidDynamic (nullptr)
	{
	}

	Rigidbody::~Rigidbody ()
	{
	}

	void Rigidbody::Destroy ()
	{
		g_physics.DeleteRigidDynamic (*this);
		
		m_physxRigidDynamic->release ();
		m_physxRigidDynamic = nullptr;
	}

	void Rigidbody::OnInit ()
	{
		m_lastUpdatePosition = Vector3::Zero;
		m_lastUpdateRotation = Quaternion::Identity;

		SetMass (m_mass);

		SetLinearDamping (m_linearDamping);
		SetAngularDamping (m_angularDamping);

		SetApplyGravity (m_bGravity);
		SetKinematic (m_bKinematic);

		SetConstraintPosition (m_bConstraintPositionX, m_bConstraintPositionY, m_bConstraintPositionZ);
		SetConstraintRotation (m_bConstraintPitch, m_bConstraintYaw, m_bConstraintRoll);

		SetVelocity (Vector3::Zero);
		SetAngularVelocity (Vector3::Zero);
	}

	void Rigidbody::OnFixedUpdate ()
	{
		// Sync physx rigid dynamic velocity
		physx::PxVec3 physxVelocity = GetRigidDynamic ()->getLinearVelocity ();
		m_velocity = Vector3 (physxVelocity.x, physxVelocity.y, physxVelocity.z);

		physx::PxVec3 physxAngularVelocity = GetRigidDynamic ()->getAngularVelocity ();
		m_angularVelocity = Vector3 (physxAngularVelocity.x, physxAngularVelocity.y, physxAngularVelocity.z);

		// Sync transform with physx actor transform
		physx::PxTransform physxTransform = GetRigidDynamic ()->getGlobalPose ();
		Vector3 physxPosition (physxTransform.p.x, physxTransform.p.y, physxTransform.p.z);
		Quaternion physxRotation (physxTransform.q.x, physxTransform.q.y, physxTransform.q.z, physxTransform.q.w);

		Transform& transform = GetGameObject ().GetTransform ();
		Vector3 position = transform.GetPosition ();
		Quaternion rotation = transform.GetRotation ();

		Vector3 updatePosition = physxPosition + position - m_lastUpdatePosition;
		Quaternion updateRotationEuler = physxRotation * (rotation * m_lastUpdateRotation.Inversed ());

		m_lastUpdatePosition = updatePosition;
		m_lastUpdateRotation = updateRotationEuler;

		transform.SetPosition (m_lastUpdatePosition);
		transform.SetRotation (m_lastUpdateRotation);

		physxTransform.p.x = m_lastUpdatePosition.m_x;
		physxTransform.p.y = m_lastUpdatePosition.m_y;
		physxTransform.p.z = m_lastUpdatePosition.m_z;

		physxTransform.q.x = m_lastUpdateRotation.m_x;
		physxTransform.q.y = m_lastUpdateRotation.m_y; 
		physxTransform.q.z = m_lastUpdateRotation.m_z; 
		physxTransform.q.w = m_lastUpdateRotation.m_w;
		
		GetRigidDynamic ()->setGlobalPose (physxTransform);
	}

	void Rigidbody::OnCollisionStart ()
	{
		for (auto& component : GetGameObject ().GetComponents<Component> ())
		{
			if (component == this)
			{
				continue;
			}

			component->OnCollisionStart ();
		}
	}

	void Rigidbody::OnCollisionFinish ()
	{
		for (auto& component : GetGameObject ().GetComponents<Component> ())
		{
			if (component == this)
			{
				continue;
			}

			component->OnCollisionFinish ();
		}
	}

	void Rigidbody::OnCollisionStay ()
	{
		for (auto& component : GetGameObject ().GetComponents<Component> ())
		{
			if (component == this)
			{
				continue;
			}

			component->OnCollisionStay ();
		}
	}

	void Rigidbody::OnTriggerStart ()
	{
		for (auto& component : GetGameObject ().GetComponents<Component> ())
		{
			if (component == this)
			{
				continue;
			}

			component->OnTriggerStart ();
		}
	}

	void Rigidbody::OnTriggerFinish ()
	{
		for (auto& component : GetGameObject ().GetComponents<Component> ())
		{
			if (component == this)
			{
				continue;
			}

			component->OnTriggerFinish ();
		}
	}

	void Rigidbody::OnTriggerStay ()
	{
		for (auto& component : GetGameObject ().GetComponents<Component> ())
		{
			if (component == this)
			{
				continue;
			}

			component->OnTriggerStay ();
		}
	}

	void Rigidbody::AddForce (Vector3 force)
	{
		GetRigidDynamic ()->addForce (physx::PxVec3 (force.m_x, force.m_y, force.m_z));
	}

	Vector3 Rigidbody::GetVelocity () const
	{
		return m_velocity;
	}

	void Rigidbody::SetVelocity (Vector3 velocity)
	{
		m_velocity = velocity;
		GetRigidDynamic ()->setLinearVelocity (physx::PxVec3 (velocity.m_x, velocity.m_y, velocity.m_z));
	}

	Vector3 Rigidbody::GetAngularVelocity () const
	{
		return m_angularVelocity;
	}

	void Rigidbody::SetAngularVelocity (Vector3 angularVelocity)
	{
		m_angularVelocity = angularVelocity;
		GetRigidDynamic ()->setAngularVelocity (physx::PxVec3 (angularVelocity.m_x, angularVelocity.m_y, angularVelocity.m_z));
	}

	float Rigidbody::GetMass () const
	{
		return m_mass;
	}

	void Rigidbody::SetMass (float mass)
	{
		m_mass = Math::Max (0.0f, mass);
		GetRigidDynamic ()->setMass (m_mass);
	}

	float Rigidbody::GetLinearDamping () const
	{
		return m_linearDamping;
	}

	void Rigidbody::SetLinearDamping (float linearDamping)
	{
		m_linearDamping = Math::Max (0.0f, linearDamping);
		GetRigidDynamic ()->setLinearDamping (m_linearDamping);
	}

	float Rigidbody::GetAngularDamping () const
	{
		return m_angularDamping;
	}

	void Rigidbody::SetAngularDamping (float angularDamping)
	{
		m_angularDamping = Math::Max (0.0f, angularDamping);
		GetRigidDynamic ()->setAngularDamping (m_angularDamping);
	}

	bool Rigidbody::IsApplyGravity () const
	{
		return m_bGravity;
	}

	void Rigidbody::SetApplyGravity (bool bGravity)
	{
		m_bGravity = bGravity;
		GetRigidDynamic ()->setActorFlag (physx::PxActorFlag::eDISABLE_GRAVITY, bGravity == false);
	}

	bool Rigidbody::IsKinematic () const
	{
		return m_bKinematic;
	}

	void Rigidbody::SetKinematic (bool bKinematic)
	{
		m_bKinematic = bKinematic;
		GetRigidDynamic ()->setRigidBodyFlag (physx::PxRigidBodyFlag::eKINEMATIC, bKinematic);
	}

	bool Rigidbody::IsConstraintPosition (ERigidbodyConstraintAxis axis) const
	{
		if (axis == ERigidbodyConstraintAxis::X)
		{
			return m_bConstraintPositionX;
		}
		else if (axis == ERigidbodyConstraintAxis::Y)
		{
			return m_bConstraintPositionY;
		}
		else if (axis == ERigidbodyConstraintAxis::Z)
		{
			return m_bConstraintPositionZ;
		}

		return false;
	}

	void Rigidbody::SetConstraintPosition (bool bPositionX, bool bPositionY, bool bPositionZ)
	{
		m_bConstraintPositionX = bPositionX;
		m_bConstraintPositionY = bPositionY;
		m_bConstraintPositionZ = bPositionZ;

		GetRigidDynamic ()->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, bPositionX);
		GetRigidDynamic ()->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, bPositionY);
		GetRigidDynamic ()->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, bPositionZ);
	}

	bool Rigidbody::IsConstraintRotation (ERigidbodyConstraintAxis axis) const
	{
		if (axis == ERigidbodyConstraintAxis::X)
		{
			return m_bConstraintPitch;
		}
		else if (axis == ERigidbodyConstraintAxis::Y)
		{
			return m_bConstraintYaw;
		}
		else if (axis == ERigidbodyConstraintAxis::Z)
		{
			return m_bConstraintRoll;
		}

		return false;
	}

	void Rigidbody::SetConstraintRotation (bool bPitch, bool bYaw, bool bRoll)
	{
		m_bConstraintPitch = bPitch;
		m_bConstraintYaw = bYaw;
		m_bConstraintRoll = bRoll;

		GetRigidDynamic ()->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, bPitch);
		GetRigidDynamic ()->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, bYaw);
		GetRigidDynamic ()->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, bRoll);
	}

	void Rigidbody::OnRenderEditor ()
	{
		SetMass (EditorGUI::InputFloat ("Mass", GetMass ()));
		SetLinearDamping (EditorGUI::InputFloat ("Linear Damping", GetLinearDamping ()));
		SetAngularDamping (EditorGUI::InputFloat ("Angular Damping", GetAngularDamping ()));

		SetApplyGravity (EditorGUI::InputCheckbox ("Apply Gravity", IsApplyGravity ()));
		SetKinematic (EditorGUI::InputCheckbox ("Is Kinematic", IsKinematic ()));

		EditorGUI::Label ("Constraints");
		bool bConstraintPositionX = EditorGUI::InputCheckbox ("Position X", IsConstraintPosition (ERigidbodyConstraintAxis::X));
		EditorGUI::SameLine ();
		bool bConstraintPositionY = EditorGUI::InputCheckbox ("Position Y", IsConstraintPosition (ERigidbodyConstraintAxis::Y));
		EditorGUI::SameLine ();
		bool bConstraintPositionZ = EditorGUI::InputCheckbox ("Position Z", IsConstraintPosition (ERigidbodyConstraintAxis::Z));
		SetConstraintPosition (bConstraintPositionX, bConstraintPositionY, bConstraintPositionZ);

		bool bConstraintRotationX = EditorGUI::InputCheckbox ("Rotation X", IsConstraintRotation (ERigidbodyConstraintAxis::X));
		EditorGUI::SameLine ();
		bool bConstraintRotationY = EditorGUI::InputCheckbox ("Rotation Y", IsConstraintRotation (ERigidbodyConstraintAxis::Y));
		EditorGUI::SameLine ();
		bool bConstraintRotationZ = EditorGUI::InputCheckbox ("Rotation Z", IsConstraintRotation (ERigidbodyConstraintAxis::Z));
		SetConstraintRotation (bConstraintRotationX, bConstraintRotationY, bConstraintRotationZ);
	}

	void Rigidbody::OnSerialize (Json::Json& json) const
	{
		Component::OnSerialize (json);

		Json::JsonSerializer::Serialize (json, "mass", m_mass);

		Json::JsonSerializer::Serialize (json, "linearDamping", m_linearDamping);
		Json::JsonSerializer::Serialize (json, "angularDamping", m_angularDamping);

		Json::JsonSerializer::Serialize (json, "gravity", m_bGravity);
		Json::JsonSerializer::Serialize (json, "kinematic", m_bKinematic);

		Json::JsonSerializer::Serialize (json, "constraintPositionX", m_bConstraintPositionX);
		Json::JsonSerializer::Serialize (json, "constraintPositionY", m_bConstraintPositionY);
		Json::JsonSerializer::Serialize (json, "constraintPositionZ", m_bConstraintPositionZ);

		Json::JsonSerializer::Serialize (json, "constraintPitch", m_bConstraintPitch);
		Json::JsonSerializer::Serialize (json, "constraintYaw", m_bConstraintYaw);
		Json::JsonSerializer::Serialize (json, "constraintRoll", m_bConstraintRoll);

		Json::JsonSerializer::Serialize (json, "velocity", m_velocity);
		Json::JsonSerializer::Serialize (json, "angularVelocity", m_angularVelocity);
	}

	void Rigidbody::OnDeserialize (const Json::Json& json)
	{
		Component::OnDeserialize (json);

		SetMass (Json::JsonSerializer::Deserialize<float> (json, "mass"));

		SetLinearDamping (Json::JsonSerializer::Deserialize<float> (json, "linearDamping"));
		SetAngularDamping (Json::JsonSerializer::Deserialize<float> (json, "angularDamping"));

		SetApplyGravity (Json::JsonSerializer::Deserialize<bool> (json, "gravity"));
		SetKinematic (Json::JsonSerializer::Deserialize<bool> (json, "kinematic"));

		SetConstraintPosition (
			Json::JsonSerializer::Deserialize<bool> (json, "constraintPositionX"),
			Json::JsonSerializer::Deserialize<bool> (json, "constraintPositionY"),
			Json::JsonSerializer::Deserialize<bool> (json, "constraintPositionZ")
		);

		SetConstraintRotation (
			Json::JsonSerializer::Deserialize<bool> (json, "constraintPitch"),
			Json::JsonSerializer::Deserialize<bool> (json, "constraintYaw"),
			Json::JsonSerializer::Deserialize<bool> (json, "constraintRoll")
		);

		SetVelocity (Json::JsonSerializer::Deserialize<Vector3> (json, "velocity"));
		SetAngularVelocity (Json::JsonSerializer::Deserialize<Vector3> (json, "angularVelocity"));
	}

	physx::PxRigidDynamic* Rigidbody::GetRigidDynamic ()
	{
		if (m_physxRigidDynamic == nullptr)
		{
			g_physics.CreateRigidDynamic (*this, Vector3::Zero, Quaternion::Identity);
		}
		
		return m_physxRigidDynamic;
	}
}