#include "Component/Physics/Rigidbody.h"
#include "Component/Physics/BoxCollider.h"
#include "Component/Physics/SphereCollider.h"
#include "Component/Transform.h"
#include "GameObject.h"

namespace GameEngine
{
	Rigidbody::Rigidbody () :
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
		m_bConstraintRoll (false)
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
		m_lastUpdatePosition = GetGameObject ().GetTransform ().GetPosition ();
		m_lastUpdateRotation = GetGameObject ().GetTransform ().GetRotation ();

		g_physics.CreateRigidDynamic (*this, m_lastUpdatePosition, m_lastUpdateRotation);

		SetMass (1.0f);

		SetLinearDamping (0.0f);
		SetAngularDamping (0.0f);

		SetApplyGravity (true);
		SetKinematic (false);

		SetConstraintPosition (false, false, false);
		SetConstraintRotation (false, false, false);

		SetVelocity (Vector3::Zero);
		SetAngularVelocity (Vector3::Zero);
	}

	void Rigidbody::OnFixedUpdate ()
	{
		// Sync physx rigid dynamic velocity
		physx::PxVec3 physxVelocity = m_physxRigidDynamic->getLinearVelocity ();
		m_velocity = Vector3 (physxVelocity.x, physxVelocity.y, physxVelocity.z);

		physx::PxVec3 physxAngularVelocity = m_physxRigidDynamic->getAngularVelocity ();
		m_angularVelocity = Vector3 (physxAngularVelocity.x, physxAngularVelocity.y, physxAngularVelocity.z);

		// Sync transform with physx actor transform
		physx::PxTransform physxTransform = m_physxRigidDynamic->getGlobalPose ();
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
		
		m_physxRigidDynamic->setGlobalPose (physxTransform);
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
		m_physxRigidDynamic->addForce (physx::PxVec3 (force.m_x, force.m_y, force.m_z));
	}

	Vector3 Rigidbody::GetVelocity () const
	{
		return m_velocity;
	}

	void Rigidbody::SetVelocity (Vector3 velocity)
	{
		m_velocity = velocity;
		m_physxRigidDynamic->setLinearVelocity (physx::PxVec3 (velocity.m_x, velocity.m_y, velocity.m_z));
	}

	Vector3 Rigidbody::GetAngularVelocity () const
	{
		return m_angularVelocity;
	}

	void Rigidbody::SetAngularVelocity (Vector3 angularVelocity)
	{
		m_angularVelocity = angularVelocity;
		m_physxRigidDynamic->setAngularVelocity (physx::PxVec3 (angularVelocity.m_x, angularVelocity.m_y, angularVelocity.m_z));
	}

	float Rigidbody::GetMass () const
	{
		return m_mass;
	}

	void Rigidbody::SetMass (float mass)
	{
		m_mass = Math::Max (0.0f, mass);
		m_physxRigidDynamic->setMass (m_mass);
	}

	float Rigidbody::GetLinearDamping () const
	{
		return m_linearDamping;
	}

	void Rigidbody::SetLinearDamping (float linearDamping)
	{
		m_linearDamping = Math::Max (0.0f, linearDamping);
		m_physxRigidDynamic->setLinearDamping (m_linearDamping);
	}

	float Rigidbody::GetAngularDamping () const
	{
		return m_angularDamping;
	}

	void Rigidbody::SetAngularDamping (float angularDamping)
	{
		m_angularDamping = Math::Max (0.0f, angularDamping);
		m_physxRigidDynamic->setAngularDamping (m_angularDamping);
	}

	bool Rigidbody::IsApplyGravity () const
	{
		return m_bGravity;
	}

	void Rigidbody::SetApplyGravity (bool bGravity)
	{
		m_bGravity = bGravity;
		m_physxRigidDynamic->setActorFlag (physx::PxActorFlag::eDISABLE_GRAVITY, bGravity == false);
	}

	bool Rigidbody::IsKinematic () const
	{
		return m_bKinematic;
	}

	void Rigidbody::SetKinematic (bool bKinematic)
	{
		m_bKinematic = bKinematic;
		m_physxRigidDynamic->setRigidBodyFlag (physx::PxRigidBodyFlag::eKINEMATIC, bKinematic);
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

		m_physxRigidDynamic->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, bPositionX);
		m_physxRigidDynamic->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, bPositionY);
		m_physxRigidDynamic->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, bPositionZ);
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

		m_physxRigidDynamic->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, bPitch);
		m_physxRigidDynamic->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, bYaw);
		m_physxRigidDynamic->setRigidDynamicLockFlag (physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, bRoll);
	}

	void Rigidbody::OnRenderEditor (Editor& editor)
	{
		editor.BeginComponent ("Rigidbody", this, true);

		float mass = GetMass ();

		editor.AddPropertyFloat ("Mass", mass);

		SetMass (mass);

		float linearDamping = GetLinearDamping ();
		float angularDamping = GetAngularDamping ();

		editor.AddPropertyFloat ("Linear Damping", linearDamping);
		editor.AddPropertyFloat ("Angular Damping", angularDamping);

		SetLinearDamping (linearDamping);
		SetAngularDamping (angularDamping);

		bool bGravity = IsApplyGravity ();
		bool bKinematic = IsKinematic ();

		editor.AddCheckbox ("Apply Gravity", bGravity);
		editor.AddCheckbox ("Is Kinematic", bKinematic);

		SetApplyGravity (bGravity);
		SetKinematic (bKinematic);

		editor.AddLabel ("Constraints");

		bool bConstraintPositionX = IsConstraintPosition (ERigidbodyConstraintAxis::X);
		bool bConstraintPositionY = IsConstraintPosition (ERigidbodyConstraintAxis::Y);
		bool bConstraintPositionZ = IsConstraintPosition (ERigidbodyConstraintAxis::Z);

		editor.AddCheckbox ("Position X", bConstraintPositionX);
		editor.AddNextItemSameLine ();
		editor.AddCheckbox ("Position Y", bConstraintPositionY);
		editor.AddNextItemSameLine ();
		editor.AddCheckbox ("Position Z", bConstraintPositionZ);

		SetConstraintPosition (bConstraintPositionX, bConstraintPositionY, bConstraintPositionZ);

		bool bConstraintRotationX = IsConstraintRotation (ERigidbodyConstraintAxis::X);
		bool bConstraintRotationY = IsConstraintRotation (ERigidbodyConstraintAxis::Y);
		bool bConstraintRotationZ = IsConstraintRotation (ERigidbodyConstraintAxis::Z);

		editor.AddCheckbox ("Rotation X", bConstraintRotationX);
		editor.AddNextItemSameLine ();
		editor.AddCheckbox ("Rotation Y", bConstraintRotationY);
		editor.AddNextItemSameLine ();
		editor.AddCheckbox ("Rotation Z", bConstraintRotationZ);

		SetConstraintRotation (bConstraintRotationX, bConstraintRotationY, bConstraintRotationZ);
	}

	//void Rigidbody::OnSerialize (Json::Json& json) const
	//{
	//	Component::OnSerialize (json);

	//	json["type"] = "Light";
	//	json["light type"] = static_cast<int32> (m_type);
	//	json["color"] = Json::JsonSerializer::Serialize<Vector4> (m_color);
	//	json["intensity"] = m_intensity;
	//	json["range"] = m_range;
	//	json["spot angle"] = m_spotAngle;
	//	json["shadow type"] = static_cast<int32> (m_shadowType);
	//	json["shadow intensity"] = m_shadowIntensity;
	//	json["shadow depth bias"] = m_shadowDepthBias;
	//}

	//void Rigidbody::OnDeserialize (const Json::Json& json)
	//{
	//}
}